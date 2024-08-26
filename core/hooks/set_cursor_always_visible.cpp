//
// Created by panzerfaust on 5/26/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <core/menu.h>

void __fastcall hooks::set_cursor_always_visible(void* ecx, void*, bool visible) {
    if (!hack::is_running)
        return o_set_cursor_always_visible(ecx, visible);

    if (menu::is_opened())
        return o_set_cursor_always_visible(ecx, false);

    o_set_cursor_always_visible(ecx, visible);
}