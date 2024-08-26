//
// Created by panzerfaust on 6/1/2020.
//

#include "chams.h"
#include "lc.h"
#include <misc/xor.h>
#include <misc/logger.h>
#include <core/cfg.h>
#include <core/game.h>

void hack::c_chams::init() {
    auto kv_default_open = new sdk::key_values;
    kv_default_open->set_name(STR("VertexLitGeneric"));
    kv_default_open->set_string(STR("$basetexture"), STR("vgui/white_additive"));
    kv_default_open->set_string(STR("$envmap"), STR(""));
    kv_default_open->set_bool(STR("$ignorez"), false);
    kv_default_open->set_bool(STR("$normalmapalphaenvmapmask"), true);
    kv_default_open->set_bool(STR("$envmapcontrast"), true);
    kv_default_open->set_bool(STR("$nofog"), true);
    kv_default_open->set_bool(STR("$model"), true);
    kv_default_open->set_bool(STR("$nocull"), false);
    kv_default_open->set_bool(STR("$selfillum"), true);
    kv_default_open->set_bool(STR("$halflambert"), true);
    kv_default_open->set_bool(STR("$znearer"), false);
    kv_default_open->set_bool(STR("$flat"), true);

    default_open = sdk::material_system->create_material(STR("tf2_default_open"), kv_default_open);
    if (!default_open)
        logger::fail(STR("Failed to create material: tf2_default_open"));

    auto kv_default_wall = new sdk::key_values;
    kv_default_wall->set_name(STR("VertexLitGeneric"));
    kv_default_wall->set_string(STR("$basetexture"), STR("vgui/white_additive"));
    kv_default_wall->set_string(STR("$envmap"), STR(""));
    kv_default_wall->set_bool(STR("$ignorez"), true);
    kv_default_wall->set_bool(STR("$normalmapalphaenvmapmask"), true);
    kv_default_wall->set_bool(STR("$envmapcontrast"), true);
    kv_default_wall->set_bool(STR("$nofog"), true);
    kv_default_wall->set_bool(STR("$model"), true);
    kv_default_wall->set_bool(STR("$nocull"), false);
    kv_default_wall->set_bool(STR("$selfillum"), true);
    kv_default_wall->set_bool(STR("$halflambert"), true);
    kv_default_wall->set_bool(STR("$znearer"), false);
    kv_default_wall->set_bool(STR("$flat"), true);

    default_wall = sdk::material_system->create_material(STR("tf2_default_wall"), kv_default_wall);
    if (!default_open)
        logger::fail(STR("Failed to create material: tf2_default_wall"));

    auto kv_flat_open = new sdk::key_values;
    kv_flat_open->set_name(STR("UnlitGeneric"));
    kv_flat_open->set_string(STR("$basetexture"), STR("vgui/white_additive"));
    kv_flat_open->set_string(STR("$envmap"), STR(""));
    kv_flat_open->set_bool(STR("$ignorez"), false);
    kv_flat_open->set_bool(STR("$normalmapalphaenvmapmask"), true);
    kv_flat_open->set_bool(STR("$envmapcontrast"), true);
    kv_flat_open->set_bool(STR("$nofog"), true);
    kv_flat_open->set_bool(STR("$model"), true);
    kv_flat_open->set_bool(STR("$nocull"), false);
    kv_flat_open->set_bool(STR("$selfillum"), true);
    kv_flat_open->set_bool(STR("$halflambert"), true);
    kv_flat_open->set_bool(STR("$znearer"), false);
    kv_flat_open->set_bool(STR("$flat"), true);

    flat_open = sdk::material_system->create_material(STR("tf2_flat_open"), kv_flat_open);
    if (!flat_open)
        logger::fail(STR("Failed to create material: tf2_flat_open"));

    auto kv_flat_wall = new sdk::key_values;
    kv_flat_wall->set_name(STR("UnlitGeneric"));
    kv_flat_wall->set_string(STR("$basetexture"), STR("vgui/white_additive"));
    kv_flat_wall->set_string(STR("$envmap"), STR(""));
    kv_flat_wall->set_bool(STR("$ignorez"), true);
    kv_flat_wall->set_bool(STR("$normalmapalphaenvmapmask"), true);
    kv_flat_wall->set_bool(STR("$envmapcontrast"), true);
    kv_flat_wall->set_bool(STR("$nofog"), true);
    kv_flat_wall->set_bool(STR("$model"), true);
    kv_flat_wall->set_bool(STR("$nocull"), false);
    kv_flat_wall->set_bool(STR("$selfillum"), true);
    kv_flat_wall->set_bool(STR("$halflambert"), true);
    kv_flat_wall->set_bool(STR("$znearer"), false);
    kv_flat_wall->set_bool(STR("$flat"), true);

    flat_wall = sdk::material_system->create_material(STR("tf2_flat_wall"), kv_flat_wall);
    if (!flat_wall)
        logger::fail(STR("Failed to create material: tf2_flat_wall"));
}

void hack::c_chams::release() {
    if (default_open)
        default_open->release();
    if (default_wall)
        default_wall->release();
}

hack::chams_result hack::c_chams::work(sdk::entity* e) {
    if (!e)
        return { false };

    chams_result r{ false };
    if (e->is_player()) {
        if (e->team() != game::me->team()) {
            if (_CFG("vis_chams_enemies_i").get<bool>(false)) {
                auto c = _CFG("vis_chams_enemies_i_col").get<_COLOR>(_COLOR_WHITE);

                r.should_draw = true;
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_players_type").get<_BITS>(0), true),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
            }

            if (_CFG("vis_chams_enemies").get<bool>(false)) {
                auto c = _CFG("vis_chams_enemies_col").get<_COLOR>(_COLOR_WHITE);

                r.should_draw = true;
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_players_type").get<_BITS>(0), false),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
            }
        } else {
            if (e->index() != game::me->index() && _CFG("vis_chams_allies").get<bool>(false)) {
                auto c = _CFG("vis_chams_allies_col").get<_COLOR>(_COLOR_WHITE);

                r.should_draw = true;
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_players_type").get<_BITS>(0), true),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_players_type").get<_BITS>(0), false),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
            } else if (e->index() == game::me->index() && _CFG("vis_chams_self").get<bool>(false)) {
                auto c = _CFG("vis_chams_self_col").get<_COLOR>(_COLOR_WHITE);

                r.should_draw = true;
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_self_type").get<_BITS>(0), true),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
                r.data.emplace_back(
                        std::make_tuple(
                                select_material(_CFG("vis_chams_self_type").get<_BITS>(0), false),
                                sdk::color(c.r(), c.g(), c.b(), c.a())
                        )
                );
            }
        }
    }

    if (e->is_object() && _CFG("vis_chams_obj").get<bool>(false)) {
        if (e->team() == game::me->team() && _CFG("vis_chams_obj_team").get<bool>(false) ||
            e->team() != game::me->team()) {
            auto c = _CFG("vis_chams_obj_col").get<_COLOR>(_COLOR_WHITE);

            r.should_draw = true;
            r.data.emplace_back(
                    std::make_tuple(
                            select_material(_CFG("vis_chams_obj_type").get<_BITS>(0), true),
                            sdk::color(c.r(), c.g(), c.b(), c.a())
                    )
            );
            r.data.emplace_back(
                    std::make_tuple(
                            select_material(_CFG("vis_chams_obj_type").get<_BITS>(0), false),
                            sdk::color(c.r(), c.g(), c.b(), c.a())
                    )
            );
        }
    }

    return r;
}

sdk::material* hack::c_chams::select_material(int i, bool is_wall) {
    if (i == 0)
        return is_wall ? default_wall : default_open;
    if (i == 1)
        return is_wall ? flat_wall : flat_open;

    return nullptr;
}

