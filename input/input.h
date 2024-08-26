//
// Created by panzerfaust on 5/18/2020.
//

#ifndef EV0LVE_RENDERER_INPUT_H
#define EV0LVE_RENDERER_INPUT_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <cstdint>
#include <unordered_map>

#include "../types/types.h"

namespace input {
    class context {
    public:
        void init(HWND _wnd);

        bool handle_message(uint32_t msg, uint32_t w, long l);
        void handle_pre_frame();
        void handle_frame();

        std::unordered_map<uint32_t, bool> keys_down;
        std::unordered_map<uint32_t, bool> keys_clicked;
        std::unordered_map<uint32_t, bool> keys_toggle;
        std::unordered_map<uint32_t, bool> keys_released;

        float wheel = 0.f;

        types::vec2 mouse;
        types::vec2 mouse_delta;
    private:
        HWND wnd = nullptr;

        std::unordered_map<uint32_t, uint32_t> keys_time;
    };

    inline context* ctx = new context;
}

#endif //EV0LVE_RENDERER_INPUT_H
