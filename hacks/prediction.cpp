//
// Created by panzerfaust on 6/8/2020.
//

#include "prediction.h"
#include <sdk/sdk.h>
#include <core/game.h>

void hack::c_prediction::work() {
    auto o_servertime = sdk::global_vars->server_time;
    auto o_frametime = sdk::global_vars->frame_time;
    auto o_tickcount = sdk::global_vars->tick_count;
    auto o_buttons = game::cmd->buttons;

    sdk::global_vars->server_time = (float)game::me->tickbase() * sdk::global_vars->tick_interval;
    sdk::global_vars->frame_time = sdk::global_vars->tick_interval;
    sdk::global_vars->tick_count = sdk::time_to_ticks(sdk::global_vars->server_time);

    *game::me->current_cmd() = game::cmd;

    sdk::move_data md{};
    sdk::game_movement->start_track_prediction_errors(game::me);
    sdk::game_prediction->setup_move(game::me, game::cmd, sdk::move_helper, &md);
    sdk::game_movement->process_movement(game::me, &md);
    sdk::game_prediction->finish_move(game::me, game::cmd, &md);
    sdk::game_movement->finish_track_prediction_errors(game::me);

    sdk::global_vars->server_time = o_servertime;
    sdk::global_vars->frame_time = o_frametime;
    sdk::global_vars->tick_count = o_tickcount;
    game::cmd->buttons = o_buttons;
}

