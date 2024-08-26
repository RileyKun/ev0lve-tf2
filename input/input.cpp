//
// Created by panzerfaust on 5/18/2020.
//

#include "input.h"

void input::context::init(HWND _wnd) {
    wnd = _wnd;
}

bool input::context::handle_message(uint32_t msg, uint32_t w, long l) {
    if (msg == WM_KEYDOWN) {
        keys_down[w] = true;
        keys_clicked[w] = true;
        keys_released[w] = false;

        return true;
    }

    if (msg == WM_KEYUP) {
        keys_down[w] = false;
        keys_clicked[w] = false;
        keys_released[w] = true;

        return true;
    }

    if (msg == WM_LBUTTONDOWN) {
        keys_down[VK_LBUTTON] = true;
        keys_clicked[VK_LBUTTON] = true;
        keys_released[VK_LBUTTON] = false;

        return true;
    }

    if (msg == WM_LBUTTONUP) {
        keys_down[VK_LBUTTON] = false;
        keys_clicked[VK_LBUTTON] = false;
        keys_released[VK_LBUTTON] = true;

        return true;
    }

    if (msg == WM_RBUTTONDOWN) {
        keys_down[VK_RBUTTON] = true;
        keys_clicked[VK_RBUTTON] = true;
        keys_released[VK_RBUTTON] = false;

        return true;
    }

    if (msg == WM_RBUTTONUP) {
        keys_down[VK_RBUTTON] = false;
        keys_clicked[VK_RBUTTON] = false;
        keys_released[VK_RBUTTON] = true;

        return true;
    }

    if (msg == WM_MBUTTONDOWN) {
        keys_down[VK_MBUTTON] = true;
        keys_clicked[VK_MBUTTON] = true;
        keys_released[VK_MBUTTON] = false;

        return true;
    }

    if (msg == WM_MBUTTONUP) {
        keys_down[VK_MBUTTON] = false;
        keys_clicked[VK_MBUTTON] = false;
        keys_released[VK_MBUTTON] = true;

        return true;
    }

    if (msg == WM_XBUTTONDOWN) {
        keys_down[VK_MBUTTON + HIWORD(w)] = true;
        keys_clicked[VK_MBUTTON + HIWORD(w)] = true;
        keys_released[VK_MBUTTON + HIWORD(w)] = false;

        return true;
    }

    if (msg == WM_XBUTTONUP) {
        keys_down[VK_MBUTTON + HIWORD(w)] = false;
        keys_clicked[VK_MBUTTON + HIWORD(w)] = false;
        keys_released[VK_MBUTTON + HIWORD(w)] = true;

        return true;
    }

    if (msg == WM_SYSKEYDOWN) {
        keys_down[VK_MENU] = true;
        keys_clicked[VK_MENU] = true;
        keys_released[VK_MENU] = false;

        return true;
    }

    if (msg == WM_SYSKEYUP) {
        keys_down[VK_MENU] = false;
        keys_clicked[VK_MENU] = false;
        keys_released[VK_MENU] = true;

        return true;
    }

    if (msg == WM_MOUSEWHEEL) {
        wheel = HIWORD(w) > 120 ? -1.f : 1.f;

        return true;
    }

    return false;
}

void input::context::handle_frame() {
    for (auto& ks : keys_clicked) {
        if (ks.second) {
            keys_toggle[ks.first] = !keys_toggle[ks.first];
            ks.second = false;
        }
    }

    for (auto& ks : keys_released) {
        if (ks.second)
            ks.second = false;
    }

    wheel = 0.f;
    mouse_delta = types::vec2();
}

void input::context::handle_pre_frame() {
    POINT cur{};
    GetCursorPos(&cur);
    ScreenToClient(wnd, &cur);

    auto old_mouse = mouse;
    mouse = types::vec2(cur.x, cur.y);
    mouse_delta = mouse - old_mouse;
}
