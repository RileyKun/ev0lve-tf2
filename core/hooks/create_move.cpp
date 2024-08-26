//
// Created by panzerfaust on 5/26/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <core/cfg.h>
#include <core/menu.h>

#include <misc/logger.h>
#include <misc/xor.h>
#include <misc/crc32.h>

#include <hacks/autojump.h>
#include <hacks/esp.h>
#include <hacks/aimbot.h>
#include <hacks/prediction.h>
#include <misc/utils.h>
#include <hacks/antiaim.h>
#include <core/game.h>

bool __fastcall hooks::create_move(void* ecx, void*, float time, sdk::user_cmd* cmd) {
    if (!hack::is_running || !cmd || !cmd->command_number)
        return hooks::o_create_move(ecx, time, cmd);

    game::send_packet = true;
    game::cmd = cmd;
    game::me = sdk::entity_list->get(sdk::engine->get_local_player());

    if (game::me) {
        hack::esp.work();

        if (game::me->life_state() == sdk::ls_alive) {
            if (menu::is_opened())
                cmd->buttons = 0;

            hack::autojump.work();
            hack::prediction.work();
            hack::aimbot.work();
            hack::antiaim.work();

            sdk::ang va;
            sdk::engine->get_view_angles(va);
            cmd->fix_movement(va);

            game::last_angle = cmd->view_angles;
        }
    }

    {
        uint32_t* frame = nullptr;
        __asm mov frame, ebp;

        *reinterpret_cast<bool*>(*frame - 0x1C) = game::send_packet;
    }

    return false;
}