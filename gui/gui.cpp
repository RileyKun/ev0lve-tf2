//
// Created by panzerfaust on 5/2/2020.
//

#include "gui.h"
#include "textures.h"
#include "fonts.h"

gui::context* gui::ctx = new gui::context;

void gui::context::init() {
    using namespace renderer;
    using namespace types;

    adapter->fonts[JOAAT("main")]       = font(menufont_regular, 443152, 12.f);
    adapter->fonts[JOAAT("tiny")]       = font(menufont_medium, 439716, 8.f);
    adapter->fonts[JOAAT("bold")]       = font(menufont_medium, 439716, 12.f);
    adapter->fonts[JOAAT("icons_24")]   = font(icons, 10304, 24.f);
    adapter->fonts[JOAAT("icons_48")]   = font(icons, 10304, 46.f);
    adapter->fonts[JOAAT("icons_86")]   = font(icons, 10304, 74.f);

    colors.background        = color(0x1D, 0x1D, 0x1D);
    colors.bottom            = color(0x12, 0x12, 0x12);
    colors.outline           = color(0x38, 0x38, 0x38);
    colors.accent            = color(0x00, 0xAC, 0xF5);
    colors.text_normal       = color(0xEF, 0xEF, 0xEF);
    colors.text_disabled     = color(0x64, 0x64, 0x64);
    colors.text_alt_1        = color(0x95, 0x95, 0x95);
    colors.text_alt_2        = color(0xBD, 0xBD, 0xBD);
    colors.even              = color(0x18, 0x18, 0x18);
    colors.odd               = color(0x14, 0x14, 0x14);
    colors.green             = color(0x57, 0xFF, 0x84);
    colors.red               = color(0xFF, 0x57, 0x57);
    colors.yellow            = color(0xFF, 0xC4, 0x57);

    texture = adapter->make_texture_memory(bg_texture, 34564);
}

void GUI_HOTKEYS_TICK(gui::element* e) {
    if (e->is_window()) {
        if (e->type == gui::gui_type::WINDOW_LARGE) {
            for (auto& c : e->as<gui::large_window>()->tabs)
                GUI_HOTKEYS_TICK(c);
        }

        for (auto& c : e->as<gui::window>()->elements)
            GUI_HOTKEYS_TICK(c);
    }

    if (e->is_tab()) {
        for (auto& c : e->as<gui::tab>()->elements)
            GUI_HOTKEYS_TICK(c);
    }

    if (e->is_layout()) {
        for (auto& c : e->as<gui::layout>()->elements)
            GUI_HOTKEYS_TICK(c);
    }

    if (e->type == gui::gui_type::CONTROL_HOTKEY)
        e->as<gui::hotkey>()->post_update();
}

void gui::context::draw() {
    for (auto wnd : windows) {
        wnd->draw();
        GUI_HOTKEYS_TICK(wnd);
    }
}

bool gui::context::is_hovered(const _RECT& r) const {
    if (active_id && active_id != current_id) return false;
    return r.contains(input::ctx->mouse);
}

bool gui::context::is_held() const {
    if (active_id && active_id != current_id) return false;
    return input::ctx->keys_down[VK_LBUTTON];
}

bool gui::context::is_released() const {
    if (active_id && active_id != current_id) return false;
    return !input::ctx->keys_down[VK_LBUTTON];
}

bool gui::context::is_clicked() const {
    if (active_id && active_id != current_id) return false;
    return input::ctx->keys_clicked[VK_LBUTTON];
}

bool gui::context::is_unclicked() const {
    if (active_id && active_id != current_id) return false;
    return input::ctx->keys_released[VK_LBUTTON];
}

bool gui::context::is_right_clicked() const {
    if (active_id && active_id != current_id) return false;
    return input::ctx->keys_clicked[VK_RBUTTON];
}

void gui::anim_color::reset() {
    progress = 0.f;
    begin = last;
}

void gui::anim_color::animate() {
    progress = std::clamp(progress, 0.f, 1.f);
    if (progress >= 1.f) {
        last = end;
        return;
    }

    progress += 1.f / duration * ctx->frame_time;
    auto x = begin.value.x < end.value.x ?
             std::clamp(interpolate(begin.value.x, end.value.x, progress), begin.value.x, end.value.x) :
             std::clamp(interpolate(begin.value.x, end.value.x, progress), end.value.x, begin.value.x);
    auto y = begin.value.y < end.value.y ?
             std::clamp(interpolate(begin.value.y, end.value.y, progress), begin.value.y, end.value.y) :
             std::clamp(interpolate(begin.value.y, end.value.y, progress), end.value.y, begin.value.y);
    auto z = begin.value.z < end.value.z ?
             std::clamp(interpolate(begin.value.z, end.value.z, progress), begin.value.z, end.value.z) :
             std::clamp(interpolate(begin.value.z, end.value.z, progress), end.value.z, begin.value.z);
    auto w = begin.value.w < end.value.w ?
             std::clamp(interpolate(begin.value.w, end.value.w, progress), begin.value.w, end.value.w) :
             std::clamp(interpolate(begin.value.w, end.value.w, progress), end.value.w, begin.value.w);
    last = _COLOR(x, y, z, w);
}

void gui::anim_val::animate() {
    progress = std::clamp(progress, 0.f, 1.f);
    if (progress >= 1.f) {
        last = end;
        return;
    }

    progress += 1.f / duration * ctx->frame_time;
    if (end > begin)
        last = std::clamp(interpolate(begin, end, progress), begin, end);
    else
        last = std::clamp(interpolate(begin, end, progress), end, begin);
}

void gui::anim_val::reset() {
    progress = 0.f;
    begin = last;
}