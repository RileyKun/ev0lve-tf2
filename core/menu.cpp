//
// Created by panzerfaust on 5/25/2020.
//

#include <core/data/cursor.h>
#include <core/menu.h>
#include <core/cfg.h>
#include <misc/xor.h>
#include <sdk/sdk.h>
#include <hacks/aimbot.h>
#include <misc/logger.h>
#include "game.h"

namespace menu {
    inline struct {
        void* cursor_image = nullptr;
    } DATA;
}

void menu::init() {
    DATA.cursor_image = renderer::adapter->make_texture_memory(DATA_IMAGE_CURSOR, 1098);

    gui::ctx->init();
    //gui::ctx->debug_mode = true;

    auto wnd = new gui::large_window(JOAAT("main_window"), _VEC2(50.f, 50.f), _VEC2(850.f, 500.f));
    wnd->tabs.emplace_back(make_tab_aim());
    wnd->tabs.emplace_back(make_tab_vis());
    wnd->tabs.emplace_back(make_tab_misc());
    wnd->tabs.emplace_back(make_tab_lua());

    gui::ctx->windows.emplace_back(wnd);

    gui::ctx->find_element<gui::tab>(JOAAT("aim"))->is_selected = true;
    gui::ctx->find_element<gui::button>(JOAAT("misc_refresh"))->callback();

    gui::ctx->find_element<gui::hotkey>(JOAAT("vis_other_tp_k"))->callback = []() {
        if (sdk::engine->is_in_game() && game::me) {
            auto hk = gui::ctx->find_element<gui::hotkey>(JOAAT("vis_other_tp_k"));
            if ((*hk->behavior == 0 || *hk->behavior == 1 || *hk->behavior == 3) && !sdk::input->is_third_person()) {
                sdk::input->is_camera_in_third_person = true;
                game::me->thirdperson_switch(true);
            }

            if (*hk->behavior == 2) {
                if (sdk::input->is_third_person())
                    sdk::input->is_camera_in_third_person = false;
                else {
                    sdk::input->is_camera_in_third_person = true;
                    game::me->thirdperson_switch(true);
                }
            }
        }
    };

    gui::ctx->find_element<gui::hotkey>(JOAAT("vis_other_tp_k"))->released = []() {
        if (sdk::engine->is_in_game() && game::me) {
            auto hk = gui::ctx->find_element<gui::hotkey>(JOAAT("vis_other_tp_k"));
            if ((*hk->behavior == 1 || *hk->behavior == 3) && sdk::input->is_third_person())
                sdk::input->is_camera_in_third_person = false;
        }
    };

    gui::ctx->find_element<gui::hotkey>(JOAAT("aim_enable_k"))->callback = []() {
        hack::aimbot.is_enabled = _CFG("aim_enable").get<bool>(false);
    };

    gui::ctx->find_element<gui::hotkey>(JOAAT("aim_enable_k"))->released = []() {
        hack::aimbot.is_enabled = false;
    };
}

void menu::draw() {
    static auto old_accent = _CFG("menu_color").get<_COLOR>(gui::ctx->colors.accent);
    auto accent = _CFG("menu_color").get<_COLOR>(gui::ctx->colors.accent);

    gui::ctx->frame_time = sdk::global_vars->frame_time;
    gui::ctx->colors.accent = accent;

    if (accent != old_accent) {
        reset_colors();
        old_accent = accent;
    }

    gui::ctx->draw();

    if (is_opened()) {
        auto d = &renderer::adapter->foreground;
        d->push_texture(DATA.cursor_image);
        d->add_rect_filled(_RECT(input::ctx->mouse, input::ctx->mouse + _VEC2(32.f, 32.f)), accent);
        d->pop_texture();
    }
}

bool menu::is_opened() {
    static auto wnd = gui::ctx->find_element<gui::large_window>(JOAAT("main_window"));
    if (!wnd) {
        wnd = gui::ctx->find_element<gui::large_window>(JOAAT("main_window"));

        if (!wnd)
            return false;
    }

    return wnd->is_visible;
}

void menu::set_opened(bool b) {
    static auto wnd = gui::ctx->find_element<gui::large_window>(JOAAT("main_window"));
    if (!wnd) {
        wnd = gui::ctx->find_element<gui::large_window>(JOAAT("main_window"));

        if (!wnd)
            return;
    }

    wnd->is_visible = b;
}

gui::layout *
menu::make_group(rage::joaat_t id, const std::string &label, const types::vec2 &pos, const types::vec2 &size,
                 gui::layout *inner) {
    auto layout = new gui::layout(id + 1, pos - _VEC2(4.f, 4.f), size + _VEC2(8.f, 8.f));
    {
        auto scroll = new gui::scroll_layout(id + 3, _VEC2(6.f, 37.f), size - _VEC2(2.f, 39.f));
        scroll->elements.emplace_back(inner);

        layout->elements.emplace_back(new gui::group(id + 2, label, size));
        layout->elements.emplace_back(scroll);
    }

    return layout;
}

inline void menu_reset_colors(gui::element* e) {
    if (e->is_window()) {
        if (e->type == gui::gui_type::WINDOW_LARGE) {
            for (auto& t : e->as<gui::large_window>()->tabs)
                menu_reset_colors(t);
        }

        for (auto& c : e->as<gui::window>()->elements)
            menu_reset_colors(c);
    }

    if (e->is_tab()) {
        for (auto& c : e->as<gui::tab>()->elements)
            menu_reset_colors(c);
    }

    if (e->is_layout()) {
        for (auto& c : e->as<gui::layout>()->elements)
            menu_reset_colors(c);
    }

    if (e->type == gui::gui_type::CONTROL_LISTBOX)
        menu_reset_colors(e->as<gui::listbox>()->elements);

    if (e->type == gui::gui_type::CONTROL_LISTBOX_ENTRY) {
        auto s = e->as<gui::listbox_entry>();
        if (s->old_state == 2) {
            s->color.end = gui::ctx->colors.accent;
            s->color.reset();
        }
    }

    if (e->type == gui::gui_type::CONTROL_COMBOBOX) {
        auto s = e->as<gui::combobox>();
        if (s->is_multiselect) {
            for (auto i = 0; i < s->elements.size(); i++) {
                if (s->value->get(i)) {
                    s->color_anim[i].end = gui::ctx->colors.accent;
                    s->color_anim[i].reset();

                    s->select_anim[i].end = 4.f;
                    s->select_anim[i].reset();
                } else {
                    s->color_anim[i].end = gui::ctx->colors.text_disabled;
                    s->color_anim[i].reset();

                    s->select_anim[i].end = 0.f;
                    s->select_anim[i].reset();
                }
            }
        } else {
            for (auto& ca : s->color_anim) {
                ca.second.end = gui::ctx->colors.text_disabled;
                ca.second.reset();
            }

            for (auto& sa : s->select_anim) {
                sa.second.end = 0.f;
                sa.second.reset();
            }

            s->color_anim[*s->value].end = gui::ctx->colors.accent;
            s->color_anim[*s->value].reset();

            s->select_anim[*s->value].end = 4.f;
            s->select_anim[*s->value].reset();
        }
    }
}

void menu::reset_colors() {
    for (auto& w : gui::ctx->windows)
        menu_reset_colors(w);
}

gui::layout *menu::make_line(rage::joaat_t id, const std::vector<gui::element *> &els) {
    auto _ = new gui::stack_layout(id, _VEC2(0.f, 0.f), _VEC2(0.f, 30.f), gui::stack_direction::HORIZONTAL);
    for (auto& e : els)
        _->elements.emplace_back(e);
    return _;
}
