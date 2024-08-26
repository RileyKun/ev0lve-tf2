//
// Created by panzerfaust on 5/28/2020.
//

#include <core/cfg.h>
#include <misc/utils.h>
#include <renderer/renderer.h>
#include <misc/xor.h>
#include <misc/logger.h>
#include <renderer/tools_math.h>
#include <core/game.h>

#include "esp.h"

void hack::c_esp::work() {
    if (!_CFG("vis_esp_enable").get<bool>(false))
        return;

    for (auto i = 1; i < sdk::global_vars->max_clients; i++) {
        if (i == game::me->index()) {
            to_erase.emplace_back(i);
            continue;
        }

        auto e = sdk::entity_list->get(i);
        if (!e) {
            to_erase.emplace_back(i);
            continue;
        }

        auto c = e->get_collideable();
        if (!c) {
            to_erase.emplace_back(i);
            continue;
        }

        if (e->team() == game::me->team() && !_CFG("vis_esp_team").get<bool>(false)) {
            to_erase.emplace_back(i);
            continue;
        }

        sdk::player_info info{};
        sdk::engine->get_player_info(e->index(), &info);

        auto p = &players[i];
        p->ent = e;
        p->pos = e->get_abs_origin();
        p->max_health = e->max_health();
        p->world_maxs = c->maxs();
        p->class_id = e->player_class();
        p->name = info.get_name();
        p->distance = floorf((game::me->origin().dist(e->origin()) * 0.02f) * (1.f / 5.f) + 0.5f) / (1.f / 5.f);
        p->is_cloaked = e->in_cond(sdk::tf_cond_stealthed);
        p->is_aiming = e->in_cond(sdk::tf_cond_aiming);
        p->is_ubercharged = e->in_cond(sdk::tf_cond_invulnerable);
        p->is_overhealed = e->in_cond(sdk::tf_cond_health_overhealed);

        auto wep = sdk::entity_list->get_from_handle(e->active_weapon());
        if (wep) {
            auto wd = wep->get_weapon_data();
            if (wd) {
                p->has_weapon = true;
                p->is_primary = wd->max_clip != -1;
                p->max_clip = p->is_primary ? wd->max_clip : wd->max_clip_sec;
                p->wep_name = sdk::localize->find(wd->print_name);

                if (!p->did_init) {
                    p->ammo.init((float)p->max_clip, 0.1f);
                    p->clip = p->max_clip;
                }

                auto ammo = wep->clip_1() == -1 ? e->ammo(1) : wep->clip_1();
                if (p->clip != ammo) {
                    p->clip = ammo;

                    p->ammo.end = (float) p->clip;
                    p->ammo.reset();
                }
            }
        } else
            p->has_weapon = false;

        if (!p->did_init) {
            p->alpha.init(255.f, 0.15f);
            p->hp.init((float)p->max_health, 0.1f);

            p->health = e->health();
            p->did_init = true;
        }

        if (p->life_state != e->life_state()) {
            p->life_state = e->life_state();

            if (p->life_state == sdk::ls_alive) {
                p->alpha.end = 255.f;
                p->alpha.reset();
            }
        }

        if (p->life_state != sdk::ls_alive)
            e->health() = 0;

        if (p->is_dormant != e->is_dormant() && p->life_state == sdk::ls_alive) {
            p->is_dormant = e->is_dormant();

            p->alpha.end = p->is_dormant ? (_CFG("vis_esp_dormant").get<bool>(false) ? 64.f : 0.f) : 255.f;
            p->alpha.reset();
        }

        if (p->health != e->health()) {
            p->health = e->health();

            p->hp.end = std::clamp(p->health, 0, p->max_health);
            p->hp.reset();

            if (p->health == 0) {
                p->alpha.end = 0.f;
                p->alpha.reset();
            }
        }

        if (p->life_state == sdk::ls_alive) {
            if (!p->is_dormant)
                p->dormancy_time = sdk::global_vars->realtime;
            else {
                if (sdk::global_vars->realtime - p->dormancy_time > 7.5f) {
                    p->dormancy_time = -1.f;

                    p->alpha.end = 0.f;
                    p->alpha.reset();
                }
            }
        }
    }
}

void hack::c_esp::draw_prepare() {
    if (!_CFG("vis_esp_enable").get<bool>(false))
        return;
    if (!sdk::engine->is_in_game())
        return;

    for (auto& i : to_erase)
        players.erase(i);
    to_erase.clear();

    for (auto& ip : players) {
        auto p = &ip.second;
        if (!p->ent)
            continue;

        auto pos_3d = p->pos;
        auto top_3d = pos_3d + sdk::vec3(0.f, 0.f, p->world_maxs.z);

        sdk::vec3 pos, top;
        if (!sdk::debug_overlay->screen_position(pos_3d, pos) && !sdk::debug_overlay->screen_position(top_3d, top)) {
            auto h = pos.y - top.y;
            auto w = h * 0.5f;

            p->mins.x = pos.x - w * 0.5f;
            p->mins.y = top.y;
            p->maxs.x = pos.x + w * 0.5f;
            p->maxs.y = pos.y;

            p->mins.floor();
            p->maxs.floor();

            p->is_offscreen = false;
        } else
            p->is_offscreen = true;
    }
}

void hack::c_esp::draw() {
    if (!_CFG("vis_esp_enable").get<bool>(false))
        return;
    if (!sdk::engine->is_in_game())
        return;

    for (auto& ip : players) {
        if (!ip.second.ent)
            continue;

        ip.second.alpha.animate();
        ip.second.hp.animate();
        ip.second.ammo.animate();

        if (!ip.second.is_offscreen) {
            draw_box(ip.second);
            draw_health(ip.second);
            draw_name(ip.second);
            draw_bottom(ip.second);
            draw_side(ip.second);
        } else
            draw_arrow(ip.second);
    }
}

void hack::c_esp::draw_box(const hack::c_esp::player &p) {
    if (!_CFG("vis_esp_box").get<bool>(false))
        return;

    auto d = &renderer::adapter->background;
    d->add_rect(_RECT(p.mins - 1.f, p.maxs + 1.f), _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
    d->add_rect(_RECT(p.mins, p.maxs), _CFG("vis_esp_box_col").get<_COLOR>(_COLOR(255, 255, 255)).mod_a(p.alpha.last / 255.f));
    d->add_rect(_RECT(p.mins + 1.f, p.maxs - 1.f), _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
}

void hack::c_esp::draw_health(const hack::c_esp::player &p) {
    if (!_CFG("vis_esp_hp").get<bool>(false))
        return;

    auto h = p.maxs.y - p.mins.y;
    auto fill = p.hp.last / (float)p.max_health * h;

    auto hp_color = p.is_overhealed ? _COLOR(69, 118, 255, (int)p.alpha.last) : _COLOR(69, 255, 118, (int)p.alpha.last);

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(p.mins - _VEC2(6.f, 2.f), p.mins - _VEC2(2.f, -(h + 1))), _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
    d->add_rect_filled(_RECT(p.mins - _VEC2(5.f, -(h - fill - 1.f)), p.mins - _VEC2(3.f, -h)), hp_color);
}

void hack::c_esp::draw_name(const hack::c_esp::player &p) {
    if (!_CFG("vis_esp_name").get<bool>(false))
        return;

    auto text_size = renderer::tools::text_size(JOAAT("esp_main"), p.name);
    auto text_pos = p.mins + _VEC2((p.maxs.x - p.mins.x) * 0.5f - text_size.x * 0.5f, -(text_size.y + 6.f));
    text_pos.floor();

    auto d = &renderer::adapter->background;
    d->add_text(JOAAT("esp_main"), p.name, text_pos + 1.f, _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
    d->add_text(JOAAT("esp_main"), p.name, text_pos,
            _CFG("vis_esp_name_col").get<_COLOR>(_COLOR(255, 255, 255)).mod_a(p.alpha.last / 255.f));
}

void hack::c_esp::draw_bottom(const hack::c_esp::player &p) {
    std::vector<bottom_info> inf;

    auto d = &renderer::adapter->background;
//    if (_CFG("vis_esp_ammo").get<bool>(false)) {
//        if (p.max_clip != -1) {
//            if (p.clip > -1) {
//                inf.emplace_back(
//                        bottom_info(0.f, (float) p.max_clip, p.ammo.last, p.clip <= 3 ? std::to_string(p.clip) : ""));
//            }
//        }
//        else {
//            if (p.clip > -1)
//                inf.emplace_back(bottom_info(STR("Ammo: ") + std::to_string(p.clip)));
//        }
//    }

    if (_CFG("vis_esp_wep").get<bool>(false))
        inf.emplace_back(bottom_info(p.wep_name));

    _VEC2 offset;
    for (auto& i : inf) {
        if (i.is_bar) {
            auto pos = p.maxs - _VEC2(p.maxs.x - p.mins.x, -(offset.y + 2.f));
            auto fill = (i.v - i.min) / (i.max - i.min) * (p.maxs.x - p.mins.x);

            d->add_rect_filled(_RECT(pos - _VEC2(1.f, 0.f), pos + _VEC2(p.maxs.x - p.mins.x + 1.f, 4.f)),
                    _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
            d->add_rect_filled(_RECT(pos + _VEC2(0.f, 1.f), pos + _VEC2(fill, 3.f)),
                               _CFG("vis_esp_ammo_col").get<_COLOR>(_COLOR(255, 255, 255)).mod_a(p.alpha.last / 255.f));

            d->add_text(JOAAT("esp_small_outline"), i.text, pos + _VEC2(fill, 0.f),
                        _COLOR(0, 0, 0).mod_a(p.alpha.last / 255.f));
            d->add_text(JOAAT("esp_small"), i.text, pos + _VEC2(fill, 0.f),
                        _COLOR(255, 255, 255).mod_a(p.alpha.last / 255.f));

            offset.y += 6.f;
        } else {
            auto text_size = renderer::tools::text_size(JOAAT("esp_main"), i.text);
            auto text_pos = p.mins + _VEC2((p.maxs.x - p.mins.x) * 0.5f - text_size.x * 0.5f, (p.maxs.y - p.mins.y) - text_size.y * 0.5f - 4.f);
            text_pos += offset;
            text_pos.floor();

            d->add_text(JOAAT("esp_main"), i.text, text_pos + 1.f, _COLOR(0, 0, 0, 150).mod_a(p.alpha.last / 255.f));
            d->add_text(JOAAT("esp_main"), i.text, text_pos,
                        _CFG("vis_esp_wep_col").get<_COLOR>(_COLOR(255, 255, 255)).mod_a(p.alpha.last / 255.f));

            offset.y += text_size.y * 0.5f - 4.f;
        }
    }
}

void hack::c_esp::draw_side(const hack::c_esp::player &p) {
    auto d = &renderer::adapter->background;
    std::vector<side_info> inf;

    auto flags_bits = _CFG("vis_esp_flags").get<_BITS>(0);
    if (flags_bits.get(0)) {
        std::string cn;
        switch (p.class_id) {
            case sdk::pc_scout: cn = STR("scout"); break;
            case sdk::pc_soldier: cn = STR("soldier"); break;
            case sdk::pc_fire: cn = STR("pyro"); break;
            case sdk::pc_demo_man: cn = STR("demoman"); break;
            case sdk::pc_heavy: cn = STR("heavy"); break;
            case sdk::pc_engineer: cn = STR("engineer"); break;
            case sdk::pc_medic: cn = STR("medic"); break;
            case sdk::pc_sniper: cn = STR("sniper"); break;
            case sdk::pc_spy: cn = STR("spy"); break;
        }

        inf.emplace_back(side_info(cn, _COLOR(255, 255, 255).mod_a(p.alpha.last / 255.f)));
    }

    if (flags_bits.get(1))
        inf.emplace_back(side_info(utils::format(STR("%.0fm"), p.distance), _COLOR(255, 255, 255).mod_a(p.alpha.last / 255.f)));

    if (flags_bits.get(2) && p.is_cloaked)
        inf.emplace_back(side_info(STR("cloak"), _COLOR(255, 50, 50).mod_a(p.alpha.last / 255.f)));

    if (flags_bits.get(3) && p.is_aiming)
        inf.emplace_back(side_info(STR("Aiming"), _COLOR(50, 50, 255).mod_a(p.alpha.last / 255.f)));

    if (flags_bits.get(4) && p.is_ubercharged)
        inf.emplace_back(side_info(STR("Über"), _COLOR(255, 125, 50).mod_a(p.alpha.last / 255.f)));

    _VEC2 offset;
    for (auto& i : inf) {
        auto pos = p.mins + _VEC2(p.maxs.x - p.mins.x + 4.f, offset.y - 8.f);
        d->add_text(JOAAT("esp_small_outline"), i.text, pos, _COLOR(0, 0, 0, 150).mod_a(i.col.a()));
        d->add_text(JOAAT("esp_small"), i.text, pos, i.col);

        offset.y += 10.f;
    }
}

void hack::c_esp::draw_arrow(const hack::c_esp::player &p) {
    if (!_CFG("vis_esp_offscreen").get<bool>(false) && game::me->life_state() == sdk::ls_alive)
        return;

    auto screen = renderer::adapter->display_size;

    sdk::ang va;
    sdk::engine->get_view_angles(va);

    auto center = _VEC2(screen.x * 0.5f, screen.y * 0.5f);
    auto rot = DEG2RAD((va.y - sdk::ang(game::me->get_abs_origin(), p.pos).y - 90.f));

    auto pos = _VEC2(center.x + 350.f * cosf(rot) * (2 * (0.5f + 10 * 0.5f * 0.01f)), center.y + 350.f * sinf(rot));
    auto line = pos - center;

    auto arrowBase = pos - (line * (28.f / (2.f * (tanf(DEG2RAD(45.f)) / 2.f) * line.len())));
    auto normal = _VEC2(-line.y, line.x);
    auto left = arrowBase + normal * (28.f / (2.f * line.len()));
    auto right = arrowBase + normal * (-28.f / (2.f * line.len()));

    auto col = _CFG("vis_esp_offscreen_col").get<_COLOR>(_COLOR_WHITE).mod_a(p.alpha.last / 255.f);
    renderer::adapter->background.add_triangle_filled(left, right, pos, col);
}

