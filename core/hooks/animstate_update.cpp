//
// Created by panzerfaust on 6/25/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>

void __fastcall hooks::update_clientside_animation(sdk::entity* ecx, void*) {
    if (!ecx->is_player())
        return o_update_clientside_animation(ecx);

    auto o_server_time = sdk::global_vars->server_time;
    auto o_frame_time = sdk::global_vars->frame_time;

    sdk::global_vars->server_time = ecx->sim_time();
    sdk::global_vars->frame_time = sdk::global_vars->tick_interval;

    o_update_clientside_animation(ecx);

    sdk::global_vars->server_time = o_server_time;
    sdk::global_vars->frame_time = o_frame_time;

    ecx->set_abs_angles(sdk::ang(0.f, hack::get<float>(ecx->anim_state(), 0x7C), 0.f));
}