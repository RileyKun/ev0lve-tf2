//
// Created by panzerfaust on 6/25/2020.
//

#include <core/cfg.h>
#include <core/game.h>
#include "antiaim.h"

void hack::c_antiaim::work() {
    if (game::cmd->buttons & sdk::in_attack || game::cmd->buttons & sdk::in_use)
        return;

    game::cmd->view_angles = sdk::ang(get_pitch(), get_yaw(), 0.f);
}

hack::antiaim_state hack::c_antiaim::get_state() {
    if (game::me->flags() & sdk::fl_on_ground) {
        if (game::me->velocity().len() < 0.1f)
            return antiaim_standing;
        return antiaim_moving;
    }

    return antiaim_air;
}

float hack::c_antiaim::get_pitch() {
    auto v = 0;
    if (get_state() == antiaim_standing)
        v = _CFG("aim_as_pitch").get<_BITS>();
    if (get_state() == antiaim_moving)
        v = _CFG("aim_am_pitch").get<_BITS>();
    if (get_state() == antiaim_air)
        v = _CFG("aim_aa_pitch").get<_BITS>();

    if (v == 1)
        return -89.f;
    if (v == 2)
        return 89.f;
    if (v == 3)
        return -179.f;
    if (v == 4)
        return 179.f;

    return game::cmd->view_angles.p;
}

float hack::c_antiaim::get_yaw() {
    auto v = 0;
    auto add = 0.f, mod = 0.f, spd = 0.f;

    if (get_state() == antiaim_standing) {
        v = _CFG("aim_as_yaw").get<_BITS>();
        add = _CFG("aim_as_yaw_add").get<float>();
        mod = _CFG("aim_as_yaw_mod").get<float>();
        spd = _CFG("aim_as_yaw_spd").get<float>();
    }

    if (get_state() == antiaim_moving) {
        v = _CFG("aim_am_yaw").get<_BITS>();
        add = _CFG("aim_am_yaw_add").get<float>();
        mod = _CFG("aim_am_yaw_mod").get<float>();
        spd = _CFG("aim_am_yaw_spd").get<float>();
    }

    if (get_state() == antiaim_air) {
        v = _CFG("aim_aa_yaw").get<_BITS>();
        add = _CFG("aim_aa_yaw_add").get<float>();
        mod = _CFG("aim_aa_yaw_mod").get<float>();
        spd = _CFG("aim_aa_yaw_spd").get<float>();
    }

    auto base = game::cmd->view_angles.y;
    if (v == 1)
        return base + add;
    if (v == 2)
        return base + 180.f + add;
    if (v == 3)
        return base + 180.f + add + (sdk::global_vars->tick_count % 2 == 0 ? mod : -mod);
    if (v == 4)
        return base + 180.f + add + (mod > 0.f ? fmod(sdk::global_vars->server_time * spd * 100.f, mod) : 0.f);

    return game::cmd->view_angles.y;
}

