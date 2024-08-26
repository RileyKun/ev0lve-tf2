//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

static std::unordered_map<uint32_t, char> SHIFT_NUM {
        { '0', ')' }, { '1', '!' }, { '2', '@' },
        { '3', '#' }, { '4', '$' }, { '5', '%' },
        { '6', '^' }, { '7', '&' }, { '8', '*' },
        { '9', '(' }
};

gui::text_input::text_input(rage::joaat_t _id, std::string *v, const types::vec2 &size) {
    id = _id;
    value = v ? v : new std::string;

    rect = _RECT(_VEC2(40.f, 5.f), size + _VEC2(40.f, 10.f));

    is_visible = true;
    is_active = true;

    hover_col.init(ctx->colors.outline, 0.15f);

    type = gui_type::CONTROL_TEXT_INPUT;
}

void gui::text_input::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 3.f), rect.maxs - _VEC2(-2.f, 3.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), ctx->colors.bottom);

    d->push_clip_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)));

    auto t = value->substr(offset);
    if (ctx->active_id == id)
        t.insert(caret - offset, "|");

    d->add_text(JOAAT("main"), t, rect.mins + _VEC2(4.f, 0.f), ctx->colors.text_normal);
    d->pop_clip_rect();

    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), hover_col.last);

    post_update();
}

void gui::text_input::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect)) {
        if (ctx->is_clicked()) {
            ctx->active_id = id;

            hover_col.end = ctx->colors.accent;
            hover_col.reset();
        } else {
            if (!is_hovered) {
                hover_col.end = ctx->colors.text_disabled;
                hover_col.reset();

                is_hovered = true;
            }
        }
    } else {
        if (ctx->active_id != id && is_hovered) {
            hover_col.end = ctx->colors.outline;
            hover_col.reset();

            is_hovered = false;
        }
    }

    if (ctx->active_id == id) {
        if (input::ctx->keys_clicked[VK_ESCAPE] || input::ctx->keys_clicked[VK_RETURN]) {
            ctx->active_id = 0;

            hover_col.end = ctx->colors.outline;
            hover_col.reset();
        } else {
            auto h_shift = input::ctx->keys_down[VK_SHIFT];
            auto h_caps = input::ctx->keys_toggle[VK_CAPITAL];

            auto o_caret = caret;
            for (auto& kv : input::ctx->keys_clicked) {
                if (!kv.second)
                    continue;

                if (kv.first == ' ') {
                    value->insert(caret, 1, (char)kv.first);
                    caret++;
                }

                if (kv.first == VK_BACK && caret > 0) {
                    value->erase(caret - 1, 1);
                    caret--;
                }

                if (kv.first == VK_DELETE && caret < value->size())
                    value->erase(caret, 1);

                if (kv.first >= '0' && kv.first <= '9') {
                    value->insert(caret, 1, h_shift ? SHIFT_NUM[kv.first] : (char)kv.first);
                    caret++;
                }

                if (kv.first >= VK_NUMPAD0 && kv.first <= VK_NUMPAD9) {
                    value->insert(caret, 1, (char)kv.first - 0x30);
                    caret++;
                }

                if (kv.first >= 'A' && kv.first <= 'Z') {
                    value->insert(caret, 1, (h_shift || h_caps) ? (char)kv.first : (char)tolower((int)kv.first));
                    caret++;
                }

                if (kv.first == VK_DIVIDE) {
                    value->insert(caret, 1, '/');
                    caret++;
                }

                if (kv.first == VK_MULTIPLY) {
                    value->insert(caret, 1, '*');
                    caret++;
                }

                if (kv.first == VK_SUBTRACT) {
                    value->insert(caret, 1, '-');
                    caret++;
                }

                if (kv.first == VK_ADD) {
                    value->insert(caret, 1, '+');
                    caret++;
                }

                if (kv.first == VK_OEM_3) {
                    value->insert(caret, 1, h_shift ? '~' : '`');
                    caret++;
                }

                if (kv.first == VK_OEM_MINUS) {
                    value->insert(caret, 1, h_shift ? '_' : '-');
                    caret++;
                }

                if (kv.first == VK_OEM_PLUS) {
                    value->insert(caret, 1, h_shift ? '+' : '=');
                    caret++;
                }

                if (kv.first == VK_OEM_4) {
                    value->insert(caret, 1, h_shift ? '{' : '[');
                    caret++;
                }

                if (kv.first == VK_OEM_6) {
                    value->insert(caret, 1, h_shift ? '}' : ']');
                    caret++;
                }

                if (kv.first == VK_OEM_5) {
                    value->insert(caret, 1, h_shift ? '|' : '\\');
                    caret++;
                }

                if (kv.first == VK_OEM_1) {
                    value->insert(caret, 1, h_shift ? ':' : ';');
                    caret++;
                }

                if (kv.first == VK_OEM_7) {
                    value->insert(caret, 1, h_shift ? '"' : '\'');
                    caret++;
                }

                if (kv.first == VK_OEM_COMMA) {
                    value->insert(caret, 1, h_shift ? '<' : ',');
                    caret++;
                }

                if (kv.first == VK_OEM_PERIOD) {
                    value->insert(caret, 1, h_shift ? '>' : '.');
                    caret++;
                }

                if (kv.first == VK_OEM_2) {
                    value->insert(caret, 1, h_shift ? '?' : '/');
                    caret++;
                }

                if (kv.first == VK_LEFT) {
                    if (caret > 0)
                        caret--;
                }

                if (kv.first == VK_RIGHT) {
                    if (caret < value->size())
                        caret++;
                }
            }

            if (o_caret != caret) {
                if (caret > o_caret) {
                    if (renderer::tools::text_size(JOAAT("main"), value->substr(offset)).x > rect.width() - 8.f)
                        offset++;
                    else {
                        if (offset > 0)
                            offset--;
                    }
                } else {
                    if (offset > 0)
                        offset--;
                }
            }
        }
    }

    if (ctx->active_id == id && ctx->is_clicked() && !ctx->is_hovered(rect)) {
        ctx->active_id = 0;

        hover_col.end = ctx->colors.outline;
        hover_col.reset();
    }
    ctx->current_id = 0;

    hover_col.animate();
}
