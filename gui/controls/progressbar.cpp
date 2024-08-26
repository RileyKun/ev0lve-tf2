//
// Created by panzerfaust on 5/5/2020.
//

#include "../gui.h"

gui::progressbar::progressbar(rage::joaat_t _id, float from, float to, const _VEC2& size) {
    id = _id;
    mins = from;
    maxs = to;

    rect = _RECT(_VEC2(40.f, 5.f), size + _VEC2(40.f, 10.f));

    is_visible = true;
    is_active = true;

    value.begin = 0.f;
    value.end = 0.f;
    value.last = 0.f;
    value.duration = 0.15f;

    type = gui_type::CONTROL_PROGRESSBAR;
}

void gui::progressbar::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 3.f), rect.maxs - _VEC2(-2.f, 3.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), ctx->colors.bottom);

    auto fill = (value.last - mins) / (maxs - mins) * rect.width();
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.mins + _VEC2(fill, rect.height() - 7.f)), ctx->colors.accent);
    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), ctx->colors.outline);

    value.animate();
    post_update();
}

void gui::progressbar::set(float v) {
    value.end = std::clamp(v, mins, maxs);
    value.reset();
}
