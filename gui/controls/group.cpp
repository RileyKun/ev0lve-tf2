//
// Created by panzerfaust on 5/3/2020.
//

#include "../gui.h"

void gui::group::draw() {
    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled_rounded(_RECT(rect.mins - _VEC2(4.f, 4.f), rect.maxs + _VEC2(4.f, 4.f)),
            _COLOR(6, 6, 6, 32), 5.f);

    d->push_clip_rect(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 30.f)));
    d->push_texture(ctx->texture);
    d->add_rect_filled_rounded(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 450.f / 800.f * rect.width())),
            _COLOR(255, 255, 255), 4.f, renderer::ROUNDING_T);
    d->pop_texture();
    d->pop_clip_rect();

    d->add_line(rect.mins + _VEC2(0.f, 30.f), rect.mins + _VEC2(rect.width(), 30.f), ctx->colors.outline);
    d->add_rect_filled_rounded(_RECT(rect.mins + _VEC2(0.f, 30.f), rect.maxs), ctx->colors.background, 4.f, renderer::ROUNDING_B);

    d->add_text(JOAAT("bold"), label, rect.mins + _VEC2(7.f, 1.f), ctx->colors.text_alt_2);

    post_update();
}

gui::group::group(rage::joaat_t _id, const std::string &name, const _VEC2& size) {
    id = _id;
    label = name;

    rect = _RECT(_VEC2(4.f, 4.f), size + _VEC2(4.f, 4.f));

    is_visible = true;
    is_active = true;

    type = gui_type::CONTROL_GROUP;
}
