//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_CORE_H
#define EV0LVE_TF_CORE_H

#include <ev0lve.h>

namespace core {
    bool init();
    void shutdown();

    bool load_interfaces();
    bool load_netvars();
    bool hook_render();
    bool hook_window();
    bool hook_game();
    bool patch_game();

    void unhook_render();
    void unhook_window();
    void unhook_game();
    void unpatch_game();

    inline struct {
        void* present = nullptr;
        void* reset = nullptr;
        void* create_move = nullptr;
        void* lock_cursor = nullptr;
        void* set_cursor_always_visible = nullptr;
        void* send_move = nullptr;
        void* run_frame = nullptr;
        void* frame_stage_notify = nullptr;
        void* fire_bullets = nullptr;
        void* draw_model_execute = nullptr;
        void* update = nullptr;
        void* override_view = nullptr;
        void* encode_float = nullptr;
        void* update_clientside_animation = nullptr;

        char patch_cam_think[10]{};
    } DATA;
}

#endif //EV0LVE_TF_CORE_H
