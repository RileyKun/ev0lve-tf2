//
// Created by panzerfaust on 5/26/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <core/menu.h>
#include <sdk/sdk.h>

void __fastcall hooks::lock_cursor(void* ecx, void*) {
    if (!hack::is_running)
        return o_lock_cursor(ecx);

    if (menu::is_opened()) {
        sdk::surface->unlock_cursor();
        return;
    }

    o_lock_cursor(ecx);
}
