//
// Created by panzerfaust on 5/12/2020.
//

#include "../gui.h"

gui::slider::slider(rage::joaat_t _id, float min, float max, float *v, float _step, const std::string& fmt, const _VEC2& size) {
    id = _id;
    mins = min;
    maxs = max;
    step = _step;
    format = fmt;

    value = v ? v : new float(0.f);
    rect = _RECT(_VEC2(40.f, 5.f), size + _VEC2(40.f, 10.f));

    is_visible = true;
    is_active = true;

    hover_col.init(ctx->colors.outline, 0.15f);
    grip_col.init(ctx->colors.text_alt_1, 0.15f);
    val_anim.init(0.f, 0.15f);

    type = gui_type::CONTROL_SLIDER;
}

void gui::slider::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect)) {
        if (ctx->is_held())
            ctx->active_id = id;

        if (!is_hovered) {
            hover_col.end = ctx->colors.text_disabled;
            hover_col.reset();

            grip_col.end = ctx->colors.text_alt_2;
            grip_col.reset();

            val_anim.end = 5.f;
            val_anim.reset();

            is_hovered = true;
        }
    } else {
        if (is_hovered && ctx->active_id != id) {
            hover_col.end = ctx->colors.outline;
            hover_col.reset();

            grip_col.end = ctx->colors.text_alt_1;
            grip_col.reset();

            val_anim.end = 0.f;
            val_anim.reset();

            is_hovered = false;
        }
    }

    if (ctx->is_hovered(_RECT(rect.mins - _VEC2(16.f, 0.f), rect.mins + _VEC2(-2.f, rect.height()))) && ctx->is_clicked()) {
        auto o = *value;
        *value = std::clamp(*value - step, mins, maxs);

        if (o != *value && callback)
            callback(*value);
    }

    if (ctx->is_hovered(_RECT(rect.mins + _VEC2(rect.width() + 2.f, 0.f), rect.mins + _VEC2(rect.width() + 16.f, rect.height()))) && ctx->is_clicked()) {
        auto o = *value;
        *value = std::clamp(*value + step, mins, maxs);

        if (o != *value && callback)
            callback(*value);
    }

    if (ctx->active_id == id) {
        auto v = (input::ctx->mouse.x - rect.mins.x) / rect.width() * (maxs - mins) + mins;
        v = floorf(v * (1.f / step) + 0.5f) / (1.f / step);

        auto o = *value;
        *value = std::clamp(v, mins, maxs);

        if (o != *value && callback)
            callback(*value);

        if (!ctx->is_held())
            ctx->active_id = 0;
    }

    ctx->current_id = 0;
    hover_col.animate();
    grip_col.animate();
    val_anim.animate();
}

void gui::slider::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 4.f), rect.maxs - _VEC2(-2.f, 8.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 6.f), rect.maxs - _VEC2(0.f, 10.f)), ctx->colors.bottom);

    auto fill = (*value - mins) / (maxs - mins) * rect.width();
    d->add_rect_filled(_RECT(
            rect.mins + _VEC2(0.f, 6.f),
            rect.mins + _VEC2(fill, rect.height() - 10.f)), ctx->colors.accent);

    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 6.f), rect.maxs - _VEC2(0.f, 10.f)), hover_col.last);

    d->add_rect_filled(_RECT(rect.mins + _VEC2(fill - 4.f, -2.f), rect.mins + _VEC2(fill + 4.f, rect.height() - 2.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(fill - 2.f, 0.f), rect.mins + _VEC2(fill + 2.f, rect.height() - 4.f)), grip_col.last);

    {
        char buffer[256]{};
        sprintf(buffer, format.c_str(), mins);

        auto text_size = renderer::tools::text_size(JOAAT("main"), buffer);
        d->add_text(JOAAT("main"), buffer, rect.mins - _VEC2(text_size.x + 8.f, 5.f), ctx->colors.text_disabled);
    }

    {
        char buffer[256]{};
        sprintf(buffer, format.c_str(), maxs);

        auto text_size = renderer::tools::text_size(JOAAT("main"), buffer);
        d->add_text(JOAAT("main"), buffer, rect.mins + _VEC2(rect.width() + 8.f, -5.f), ctx->colors.text_disabled);
    }

    char buffer[256]{};
    sprintf(buffer, format.c_str(), *value);

    auto text_size = renderer::tools::text_size(JOAAT("bold"), buffer);

    auto hover_rect = _RECT(
            (rect.mins + _VEC2(fill - text_size.x * 0.5f - 4.f, rect.height() + val_anim.last - 5.f)).floor(),
            (rect.mins + _VEC2(fill + text_size.x * 0.5f + 4.f, rect.height() + 13.f + val_anim.last)).floor());

    auto a = is_hovered ? val_anim.progress : 1.f - val_anim.progress;

    auto f = &renderer::adapter->foreground;
    f->add_rect_filled(hover_rect.expand(2.f), _COLOR(6, 6, 6, 64).mod_a(a));
    f->add_rect_filled(hover_rect, ctx->colors.bottom.mod_a(a));
    f->add_text(JOAAT("bold"), buffer,
            (rect.mins + _VEC2(fill - text_size.x * 0.5f, rect.height() + val_anim.last - 10.f)).floor(),
            ctx->colors.text_normal.mod_a(a));
    f->add_rect(hover_rect, ctx->colors.outline.mod_a(a));

    post_update();
}
