//
// Created by panzerfaust on 6/7/2020.
//

#include <core/cfg.h>
#include "aimbot.h"
#include "lc.h"
#include <misc/logger.h>
#include <core/game.h>

void hack::c_aimbot::work() {
    if (!is_enabled)
        return;

    auto target = get_target();
    if (!target.can_hit)
        return;

    aim(target);
}

hack::aim_target hack::c_aimbot::get_target() {
    auto best_fov = 360.f;
    auto best_ent = aim_target{};
    auto wep = sdk::entity_list->get_from_handle(game::me->active_weapon());
    if (!wep)
        return best_ent;

    auto wep_data = wep->get_weapon_data();
    if (!wep_data)
        return best_ent;

    for (auto i = 1; i < sdk::global_vars->max_clients; i++) {
        auto e = sdk::entity_list->get(i);
        if (!e ||
            e->team() == game::me->team() ||
            e->life_state() != sdk::ls_alive ||
            e->is_dormant() ||
            !e->is_player() ||
            e->in_cond(sdk::tf_cond_invulnerable))
            continue;

        if (wep_data->is_melee && e->origin().dist(game::me->origin()) > 64.f)
            continue;

        auto fov = game::cmd->view_angles.fov(sdk::ang(game::me->get_abs_origin(), e->get_abs_origin()));
        auto blt = aim_target{};

        if (fov < best_fov && fov <= _CFG("aim_fov").get<float>(180.f) && can_hit(e, blt)) {
            best_ent = blt;
            best_fov = fov;
        }
    }

    return best_ent;
}

bool hack::c_aimbot::can_hit(sdk::entity* e, aim_target& b) {
    auto v = _CFG("aim_hb").get<_BITS>(0);
    auto l = lc.get(e);
    if (l.empty())
        return false;
    if (l.size() == 1 && l.back().id == 0)
        return false;

    std::vector<lc_record> records;
    records.emplace_back(l.back());

    for (auto& r : records) {
        std::vector<aim_bullet> bullets;
        if (v.get(0)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_head, r.bones) });
        }

        if (v.get(1)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_spine_1, r.bones) });
        }

        if (v.get(2)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_pelvis, r.bones) });
        }

        if (v.get(3)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_hand_l, r.bones) });
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_hand_r, r.bones) });
        }

        if (v.get(4)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_knee_l, r.bones) });
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_knee_r, r.bones) });
        }

        if (v.get(5)) {
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_foot_l, r.bones) });
            bullets.emplace_back(aim_bullet{ e->get_hitbox_pos(sdk::hb_foot_r, r.bones) });
        }

        for (auto& a : bullets) {
            sdk::trace tr{};
            game::func::util_traceline(
                    game::me->get_abs_origin() + game::me->view_offset(), a.pos, sdk::mask_shot, game::me, 21, &tr
            );

            if (tr.fraction != 1.f) {
                if (tr.ent && tr.ent->index() != e->index())
                    continue;

                b.pos = a.pos;
                b.can_hit = true;
                b.entity = e;

                return true;
            }
        }
    }

    return false;
}

void hack::c_aimbot::aim(aim_target& b) {
    auto ang = sdk::ang(game::me->get_abs_origin() + game::me->view_offset(), b.pos);
    auto wep = sdk::entity_list->get_from_handle(game::me->active_weapon());
    if (!wep)
        return;

    if (wep->get_weapon_data()->is_melee && game::me->origin().dist(b.pos) <= 64.f)
        return;

    if (_CFG("aim_autofire").get<bool>(false) &&
        wep->can_attack() &&
        wep->next_primary_attack() <= sdk::global_vars->server_time)
        game::cmd->buttons |= sdk::in_attack;

    if (_CFG("aim_silent").get<bool>(false)) {
        if (game::cmd->buttons & sdk::in_attack) {
            game::send_packet = false;
            game::cmd->view_angles = ang;
        }
    } else {
        sdk::engine->set_view_angles(ang);
        game::cmd->view_angles = ang;
    }
}