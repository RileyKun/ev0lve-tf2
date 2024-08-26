//
// Created by panzerfaust on 5/6/2020.
//

#include "../gui.h"

gui::listbox_product_entry::listbox_product_entry(rage::joaat_t _id, const gui::data::product &product) {
    id = _id;
    info = product;

    is_visible = true;
    is_active = true;

    rect = _RECT(_VEC2(0.f, 0.f), _VEC2(0.f, 40.f));

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

void gui::listbox_product_entry::draw() {
    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(rect, is_even ? ctx->colors.even : ctx->colors.odd);
    d->add_rect_filled(_RECT(rect.mins, rect.mins + _VEC2(offset.last, rect.height())), ctx->colors.accent);

    d->add_text(JOAAT("bold"), info.name, rect.mins + _VEC2(offset.last + 5.f, -2.f), color.last);
    d->add_text(JOAAT("main"), "Status: ", rect.mins + _VEC2(offset.last + 5.f, 16.f), ctx->colors.text_alt_1);

    if (info.status == gui::data::product_status::STATUS_ONLINE)
        d->add_text(JOAAT("bold"), "Online", rect.mins + _VEC2(offset.last + 45.f, 16.f), ctx->colors.green);
    if (info.status == gui::data::product_status::STATUS_OFFLINE)
        d->add_text(JOAAT("bold"), info.custom_message.empty() ? "Offline" : info.custom_message,
                rect.mins + _VEC2(offset.last + 45.f, 16.f), info.custom_message.empty() ? ctx->colors.red : ctx->colors.yellow);

    d->add_text(JOAAT("main"), "Updated: ", rect.mins + _VEC2(offset.last + 115.f, 16.f), ctx->colors.text_alt_1);
    d->add_text(JOAAT("bold"), info.last_update, rect.mins + _VEC2(offset.last + 165.f, 16.f), ctx->colors.text_alt_1);

    post_update();
}
