//
// Created by panzerfaust on 5/30/2020.
//

#include <core/hooks.h>
#include <sdk/sdk.h>
#include <hacks/lc.h>
#include <core/game.h>

void __fastcall hooks::frame_stage_notify(void* ecx, void*, int stage) {
    if (!hack::is_running)
        return o_frame_stage_notify(ecx, stage);
    if (!sdk::engine->is_in_game())
        return;

    if (stage == sdk::fs_render_start) {
        if (game::me && game::me->life_state() == sdk::ls_alive && sdk::input->is_third_person())
            game::me->view_angle() = game::last_angle;
    }

    static auto old_tickcount = 0;
    if (stage == sdk::fs_net_update_end && old_tickcount != sdk::global_vars->tick_count) {
        hack::lc.work();

        if (game::me && game::me->life_state() == sdk::ls_alive && sdk::input->is_third_person()) {
            auto as = game::me->anim_state();
            as->last_aim_turn_time = 0.f;
            as->pp_last_aim_turn_time = 0.f;
            as->pp_estimate_yaw = 0.f;
            as->current_feet_yaw = as->eye_yaw;
        }

        old_tickcount = sdk::global_vars->tick_count;
    }

    o_frame_stage_notify(ecx, stage);
}