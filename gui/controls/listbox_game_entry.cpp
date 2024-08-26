//
// Created by panzerfaust on 5/6/2020.
//

#include "../gui.h"

gui::listbox_game_entry::listbox_game_entry(rage::joaat_t _id, const gui::data::game &game) {
    id = _id;
    info = game;

    is_visible = true;
    is_active = true;

    rect = _RECT(_VEC2(0.f, 0.f), _VEC2(0.f, 40.f));

    if (info.texture && info.texture_size > 0)
        texture = renderer::adapter->make_texture_memory(info.texture, info.texture_size);

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

void gui::listbox_game_entry::draw() {
    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(rect, is_even ? ctx->colors.even : ctx->colors.odd);
    d->add_rect_filled(_RECT(rect.mins, rect.mins + _VEC2(offset.last, rect.height())), ctx->colors.accent);

    if (texture) {
        // d->AddImage(texture,
        //             rect.Min + ImVec2(offset.last + 4.f, 4.f),
        //             rect.Min + ImVec2(offset.last + 36.f, 36.f));
    }

    d->add_text(JOAAT("bold"), info.name, rect.mins + _VEC2(offset.last + 41.f, -2.f), color.last);
    d->add_text(JOAAT("main"), "Products: " + std::to_string(info.product_count), rect.mins + _VEC2(offset.last + 41.f, 16.f),
            ctx->colors.text_alt_1);

    post_update();
}
