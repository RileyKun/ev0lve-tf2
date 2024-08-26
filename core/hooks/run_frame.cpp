//
// Created by panzerfaust on 5/28/2020.
//

#include <core/hooks.h>
#include <hacks/esp.h>
#include <sdk/sdk.h>
#include <core/game.h>

void __fastcall hooks::run_frame(void* ecx, void*) {
    if (!hack::is_running)
        return o_run_frame(ecx);

    hack::esp.draw_prepare();
    o_run_frame(ecx);

    if (!sdk::engine->is_in_game()) {
        hack::esp.reset();

        game::cmd = nullptr;
        game::me = nullptr;
    }
}