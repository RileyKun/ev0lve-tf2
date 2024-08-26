//
// Created by panzerfaust on 5/3/2020.
//

#include "../gui.h"

gui::checkbox::checkbox(rage::joaat_t _id, const std::string &name, bool *vptr) {
    id = _id;
    label = name;

    if (!vptr)
        value = new bool(false);
    else
        value = vptr;

    rect = _RECT(_VEC2(10.f, 5.f), _VEC2(0.f, 0.f));

    outline.begin = ctx->colors.outline;
    outline.end = ctx->colors.outline;
    outline.last = ctx->colors.outline;
    outline.duration = 0.15f;

    is_visible = true;
    is_active = true;

    type = gui_type::CONTROL_CHECKBOX;
}

void gui::checkbox::update() {
    if (!set_rect) {
        rect.maxs = rect.mins + _VEC2(40.f + renderer::tools::text_size(JOAAT("main"), label).x, 25.f);
        set_rect = true;
    }

    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect) && is_in_bounds) {
        if (ctx->is_clicked()) {
            *value = !*value;
            ctx->active_id = id;

            if (callback)
                callback(*value);
        }

        if (!was_hovered) {
            outline.end = ctx->colors.text_disabled;
            outline.reset();

            was_hovered = true;
        }
    } else {
        if (was_hovered) {
            outline.end = ctx->colors.outline;
            outline.reset();

            was_hovered = false;
        }
    }

    if (ctx->is_released() && ctx->active_id == id)
        ctx->active_id = 0;
    ctx->current_id = 0;

    outline.animate();
}

void gui::checkbox::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(2.f, 1.f), rect.mins + _VEC2(18.f, 18.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(5.f, 5.f), rect.mins + _VEC2(15.f, 15.f)), ctx->colors.bottom);
    d->add_rect(_RECT(rect.mins + _VEC2(4.f, 4.f), rect.mins + _VEC2(16.f, 16.f)), outline.last);

    if (value && *value) {
        d->add_line(rect.mins + _VEC2(5.f, 9.f), rect.mins + _VEC2(9.f, 13.f), _COLOR(6, 6, 6, 196), 4.f);
        d->add_line(rect.mins + _VEC2(8.f, 12.f), rect.mins + _VEC2(15.f, 5.f), _COLOR(6, 6, 6, 196), 4.f);

        d->add_line(rect.mins + _VEC2(5.f, 9.f), rect.mins + _VEC2(9.f, 13.f), ctx->colors.accent, 2.f);
        d->add_line(rect.mins + _VEC2(8.f, 13.f), rect.mins + _VEC2(15.f, 6.f), ctx->colors.accent, 2.f);
    }

    d->add_text(JOAAT("main"), label, rect.mins + _VEC2(30.f, -4.f), ctx->colors.text_normal);

    post_update();
}
