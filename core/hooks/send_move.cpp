//
// Created by panzerfaust on 5/27/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <sdk/sdk.h>
#include <misc/xor.h>
#include <misc/mem.h>
#include <misc/utils.h>
#include <misc/logger.h>

void write_user_cmd(sdk::bf_write* buf, const sdk::user_cmd* t, const sdk::user_cmd* f) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? 83 C4 0C 80 7B 10 00"))
            .scan(mem::module(STR("client.dll")))
            .add(1)
            .rip()
            .as<void(*)(sdk::bf_write*, const sdk::user_cmd*, const sdk::user_cmd*)>();

    fn(buf, t, f);
}

void __stdcall hooks::send_move() {
    static auto sv_maxusrcmdprocessticks = sdk::cvar->find(STR("sv_maxusrcmdprocessticks"));

    char data[4000]{};
    auto next_command_nr = sdk::client_state->last_outgoing_command + sdk::client_state->choked_commands + 1;

    sdk::msg_move move;
    move.data_out.start_writing(data, sizeof(data));
    move.backup_commands = 2;
    move.new_commands = std::clamp(1 + sdk::client_state->choked_commands, 0, sv_maxusrcmdprocessticks->get_int());

    auto num_cmds = move.new_commands + move.backup_commands;
    auto from = -1;
    auto ok = true;

    for (auto to = next_command_nr - num_cmds + 1; to <= next_command_nr; to++) {
        ok = ok && sdk::client->write_usercmd(&move.data_out, from, to, to >= (next_command_nr - move.new_commands + 1));
        from = to;
    }

    if (ok)
        sdk::client_state->channel->send_net_msg(move, false, false);
}