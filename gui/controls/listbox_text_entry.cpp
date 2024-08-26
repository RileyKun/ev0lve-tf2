//
// Created by panzerfaust on 5/4/2020.
//

#include "../gui.h"

gui::listbox_text_entry::listbox_text_entry(rage::joaat_t _id, const std::string& name) {
    id = _id;
    label = name;

    is_visible = true;
    is_active = true;

    rect = _RECT(_VEC2(0.f, 0.f), _VEC2(0.f, 20.f));

    offset.begin = 0.f;
    offset.end = 0.f;
    offset.last = 0.f;
    offset.duration = 0.15f;

    color.begin = ctx->colors.text_disabled;
    color.end = ctx->colors.text_disabled;
    color.last = ctx->colors.text_disabled;
    color.duration = 0.15f;

    type = gui_type::CONTROL_LISTBOX_ENTRY;
}

void gui::listbox_text_entry::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(rect, is_even ? ctx->colors.even : ctx->colors.odd);
    d->add_rect_filled(_RECT(rect.mins, rect.mins + _VEC2(offset.last, rect.height())), ctx->colors.accent);
    d->add_text(JOAAT("main"), label, rect.mins + _VEC2(offset.last + 5.f, -4.f), color.last);

    post_update();
}
