//
// Created by panzerfaust on 5/25/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <core/menu.h>
#include <input/input.h>

long __stdcall hooks::wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
    if (!renderer::adapter || !hack::is_running)
        return CallWindowProcA(o_wndproc, hwnd, msg, w, l);

    if (msg == WM_QUIT) {
        hack::is_running = false;
        return 0;
    }

    if (input::ctx->handle_message(msg, w, l) && menu::is_opened()) {
        LoadCursorA(nullptr, nullptr);
        return 0;
    }

    return CallWindowProcA(o_wndproc, hwnd, msg, w, l);
}