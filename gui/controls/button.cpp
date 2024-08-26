//
// Created by panzerfaust on 5/5/2020.
//

#include "../gui.h"

gui::button::button(rage::joaat_t _id, const std::string &name, const types::vec2& size) {
    id = _id;
    label = name;

    rect = _RECT(_VEC2(40.f, 5.f), _VEC2(40.f, 10.f) + size);

    is_visible = true;
    is_active = true;

    color.begin = ctx->colors.outline;
    color.end = ctx->colors.outline;
    color.last = ctx->colors.outline;
    color.duration = 0.15f;

    type = gui_type::CONTROL_BUTTON;
}

void gui::button::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect) && is_in_bounds) {
        if (!is_hovered) {
            color.end = ctx->colors.text_disabled;
            color.reset();

            is_hovered = true;
        }

        if (ctx->is_held())
            ctx->active_id = id;
    } else {
        if (is_hovered) {
            color.end = ctx->colors.outline;
            color.reset();

            is_hovered = false;
        }
    }

    if (ctx->active_id == id) {
        is_pressed = true;

        if (ctx->is_released()) {
            if (callback)
                callback();

            ctx->active_id = 0;
        }
    } else
        is_pressed = false;

    ctx->current_id = 0;

    color.animate();
}

void gui::button::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins - _VEC2(2.f, 3.f), rect.maxs + _VEC2(2.f, -3.f)), _COLOR(6, 6, 6, 64));

    if (!is_pressed)
        d->add_rect_filled_multicolor(_RECT(rect.mins, rect.maxs - _VEC2(0.f, 5.f)),
                ctx->colors.background, ctx->colors.background,
                ctx->colors.bottom, ctx->colors.bottom);
    else
        d->add_rect_filled_multicolor(_RECT(rect.mins, rect.maxs - _VEC2(0.f, 5.f)),
                                   ctx->colors.bottom, ctx->colors.bottom,
                                   ctx->colors.background, ctx->colors.background);

    d->add_rect(_RECT(rect.mins, rect.maxs - _VEC2(0.f, 5.f)), color.last);
    d->add_text(JOAAT("main"), label, (rect.mins + _VEC2(rect.width() * 0.5f,
              (rect.height() - 5.f) * 0.5f - 5.f) - renderer::tools::text_size(JOAAT("main"), label) * 0.5f).round(),
              ctx->colors.text_normal);

    post_update();
}
