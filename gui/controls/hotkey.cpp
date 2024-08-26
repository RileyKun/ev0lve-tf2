//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

static std::vector<std::string> HOTKEY_B {
    "Enable", "Hold", "Toggle", "Release"
};

static std::unordered_map<uint32_t, std::string> HOTKEY_K {
        { VK_LBUTTON, "M1" }, { VK_RBUTTON, "M2" }, { VK_MBUTTON, "M3" }, { VK_XBUTTON1, "M4" },
        { VK_XBUTTON2, "M5" }, { VK_BACK, "BACK" }, { VK_TAB, "TAB" }, { VK_RETURN, "RETURN" },
        { VK_SHIFT, "SHIFT" }, { VK_CONTROL, "CTRL" }, { VK_MENU, "ALT" }, { VK_PAUSE, "PAUSE" },
        { VK_CAPITAL, "CAPS" }, { VK_ESCAPE, "ESC" }, { VK_SPACE, "SPACE" }, { VK_PRIOR, "PG UP" },
        { VK_NEXT, "PG DN" }, { VK_END, "END" }, { VK_HOME, "HOME" }, { VK_LEFT, "LEFT" },
        { VK_UP, "UP" }, { VK_RIGHT, "RIGHT" }, { VK_DOWN, "DOWN" }, { VK_SNAPSHOT, "PR SC" },
        { VK_INSERT, "INSERT" }, { VK_DELETE, "DELETE" }, { '0', "0" }, { '1', "1" },
        { '2', "2" }, { '3', "3" }, { '4', "4" }, { '5', "5" },
        { '6', "6" }, { '7', "7" }, { '8', "8" }, { '9', "9" },
        { 'A', "A" }, { 'B', "B" }, { 'C', "C" }, { 'D', "D" },
        { 'E', "E" }, { 'F', "F" }, { 'G', "G" }, { 'H', "H" },
        { 'I', "I" }, { 'J', "J" }, { 'K', "K" }, { 'L', "L" },
        { 'M', "M" }, { 'N', "N" }, { 'O', "O" }, { 'P', "P" },
        { 'Q', "Q" }, { 'R', "R" }, { 'S', "S" }, { 'T', "T" },
        { 'U', "U" }, { 'V', "V" }, { 'W', "W" }, { 'X', "X" },
        { 'Y', "Y" }, { 'Z', "Z" }, { VK_LWIN, "WIN" }, { VK_RWIN, "WIN" },
        { VK_NUMPAD0, "KP0" }, { VK_NUMPAD1, "KP1" }, { VK_NUMPAD2, "KP2" }, { VK_NUMPAD3, "KP3" },
        { VK_NUMPAD4, "KP4" }, { VK_NUMPAD5, "KP5" }, { VK_NUMPAD6, "KP6" }, { VK_NUMPAD7, "KP7" },
        { VK_NUMPAD8, "KP8" }, { VK_NUMPAD9, "KP9" }, { VK_MULTIPLY, "*" }, { VK_ADD, "+" },
        { VK_SUBTRACT, "-" }, { VK_DECIMAL, "." }, { VK_DIVIDE, "/" }, { VK_F1, "F1" },
        { VK_F2, "F2" }, { VK_F3, "F3" }, { VK_F4, "F4" }, { VK_F5, "F5" },
        { VK_F6, "F6" }, { VK_F7, "F7" }, { VK_F8, "F8" }, { VK_F9, "F9" },
        { VK_F10, "F10" }, { VK_F11, "F11" }, { VK_F12, "F12" }, { VK_NUMLOCK, "NUM" },
        { VK_SCROLL, "SCR" }, { VK_OEM_1, ";" }, { VK_OEM_PLUS, "=" }, { VK_OEM_COMMA, "," },
        { VK_OEM_MINUS, "-" }, { VK_OEM_PERIOD, "." }, { VK_OEM_2, "/" }, { VK_OEM_3, "~" },
        { VK_OEM_4, "[" }, { VK_OEM_5, "\\" }, { VK_OEM_6, "]" }, { VK_OEM_7, "'" }
};

gui::hotkey::hotkey(rage::joaat_t _id, uint32_t *v, uint32_t *b) {
    id = _id;

    rect = _RECT(0.f, 5.f, 35.f, 25.f);

    value = v ? v : new uint32_t(0);
    behavior = b;

    is_visible = true;
    is_active = true;

    open_anim.init(-5.f, 0.1f);
    hover_col.init(ctx->colors.text_disabled, 0.15f);

    for (auto i = 0; i < HOTKEY_B.size(); i++)
        color_anim[i].init(ctx->colors.text_disabled, 0.15f);

    type = gui_type::CONTROL_HOTKEY;
}

void gui::hotkey::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->active_id == id) {
        if (input::ctx->keys_clicked[VK_ESCAPE]) {
            *value = 0;

            hover_col.end = ctx->colors.text_disabled;
            hover_col.reset();

            ctx->active_id = 0;
        } else {
            for (auto& kv : input::ctx->keys_clicked) {
                if (kv.second) {
                    *value = kv.first;

                    hover_col.end = ctx->colors.text_disabled;
                    hover_col.reset();

                    ctx->active_id = 0;
                }
            }
        }
    }

    if (ctx->is_hovered(rect)) {
        if (ctx->is_clicked()) {
            hover_col.end = ctx->colors.accent;
            hover_col.reset();

            ctx->active_id = id;
        }

        if (!is_hovered) {
            hover_col.end = ctx->colors.text_normal;
            hover_col.reset();

            is_hovered = true;
        }
    } else {
        if (is_hovered && ctx->active_id != id) {
            hover_col.end = ctx->colors.text_disabled;
            hover_col.reset();

            is_hovered = false;
        }
    }

    ctx->current_id = id + 1;
    if (ctx->is_hovered(rect) && behavior) {
        if (ctx->is_right_clicked() && ctx->active_id != id + 1) {
            ctx->active_id = id + 1;

            open_anim.end = 0.f;
            open_anim.reset();
        }
    }

    if (ctx->active_id == id + 1 && behavior) {
        for (auto i = 0; i < HOTKEY_B.size(); i++) {
            auto is_selected = *behavior == i;
            auto dr = _RECT(dropdown.mins + _VEC2(0.f, i * 20.f), dropdown.mins + _VEC2(dropdown.width(), i * 20.f + 19.f));

            if (ctx->is_hovered(dr)) {
                if (!ctx->is_unclicked()) {
                    if (!hovered[i] && !is_selected) {
                        hovered[i] = true;

                        color_anim[i].end = ctx->colors.text_normal;
                        color_anim[i].reset();
                    }
                } else {
                    *behavior = i;

                    for (auto k = 0; k < HOTKEY_B.size(); k++) {
                        color_anim[k].end = k == *behavior ? ctx->colors.accent : ctx->colors.text_disabled;
                        color_anim[k].reset();
                    }
                }
            } else {
                if (hovered[i] && !is_selected) {
                    hovered[i] = false;

                    color_anim[i].end = ctx->colors.text_disabled;
                    color_anim[i].reset();
                }
            }
        }
    }

    if (ctx->is_unclicked() && ctx->active_id == id + 1) {
        ctx->active_id = 0;

        open_anim.end = -5.f;
        open_anim.reset();
    }

    ctx->current_id = 0;

    hover_col.animate();
    for (auto i = 0; i < HOTKEY_B.size(); i++)
        color_anim[i].animate();
    open_anim.animate();

    dropdown = _RECT(
            rect.mins + _VEC2(0.f, rect.height() + open_anim.last + 5.f),
            rect.mins + _VEC2(80.f, rect.height() + open_anim.last + HOTKEY_B.size() * 20.f + 5.f));
}

void gui::hotkey::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 3.f), rect.maxs - _VEC2(-2.f, 3.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 5.f)), ctx->colors.bottom);
    d->add_text(JOAAT("tiny"), HOTKEY_K[*value], rect.mins + _VEC2(4.f, -6.f), hover_col.last);
    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 5.f)), hover_col.last);

    if ((ctx->active_id == id + 1 || open_anim.progress < 1.f) && behavior) {
        auto _a = ctx->active_id == (id + 1) ? open_anim.progress : (1.f - open_anim.progress);

        d = &renderer::adapter->foreground;
        d->add_rect_filled(dropdown.expand(2.f), _COLOR(6, 6, 6, 64)
                .mod_a((uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled(dropdown, ctx->colors.bottom.mod_a(_a));

        for (auto i = 0; i < HOTKEY_B.size(); i++) {
            auto is_selected = *behavior == i;
            auto dr = _RECT(dropdown.mins + _VEC2(0.f, i * 20.f), dropdown.mins + _VEC2(dropdown.width(), i * 20.f + 20.f));

            d->add_rect_filled(dr, i % 2 == 0 ?
                                   ctx->colors.even.mod_a(_a) :
                                   ctx->colors.odd.mod_a(_a));
            d->add_rect_filled(_RECT(dr.mins, dr.mins + _VEC2(is_selected ? 4.f : 0.f, dr.height())),
                               is_selected ? ctx->colors.accent.mod_a(_a) : color_anim[i].last.mod_a(_a));
            d->add_text(is_selected ? JOAAT("bold") : JOAAT("main"), HOTKEY_B.at(i), dr.mins + _VEC2((is_selected ? 4.f : 0.f) + 4.f, -4.f),
                        is_selected ? ctx->colors.accent.mod_a(_a) : color_anim[i].last.mod_a(_a));
        }

        d->add_rect(dropdown, ctx->colors.outline.mod_a(_a));
    }
}

void gui::hotkey::post_update() {
    if (!callback)
        return;

    auto b = behavior ? *behavior : 1;
    if (!b)
        callback();

    if (b == 1) {
        if (input::ctx->keys_down[*value])
            callback();
        else {
            if (released)
                released();
        }
    }

    if (b == 2 && input::ctx->keys_clicked[*value])
        callback();

    if (b == 3) {
        if (!input::ctx->keys_down[*value])
            callback();
        else {
            if (released)
                released();
        }
    }
}
