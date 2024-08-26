//
// Created by panzerfaust on 5/4/2020.
//

#include "../gui.h"

gui::listbox::listbox(rage::joaat_t _id, const _VEC2& size) {
    id = _id;

    is_visible = true;
    is_active = true;

    rect = _RECT(_VEC2(40.f, 10.f), _VEC2(40.f, 20.f) + size);

    background = new scroll_layout(_id, _VEC2(0.f, 0.f), size - _VEC2(1.f, 1.f));
    elements = new stack_layout(_id + 1, _VEC2(0.f, 0.f), size - _VEC2(1.f, 1.f));

    background->elements.push_back(elements);

    type = gui_type::CONTROL_LISTBOX;
}

void gui::listbox::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;
}

void gui::listbox::draw() {
    update();

    auto d = &renderer::adapter->background;
    rect.maxs.y -= 10.f;
    d->add_rect_filled(_RECT(rect.mins - _VEC2(2.f, 3.f), rect.maxs + _VEC2(2.f, 2.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(rect, ctx->colors.bottom);

    auto is_even = true;
    for (auto& e : elements->elements) {
        if (e->type == gui_type::CONTROL_LISTBOX_ENTRY) {
            auto entry = (listbox_entry *) e;
            entry->parent = this;
            entry->is_even = is_even;
            entry->rect.maxs.x = rect.width();

            is_even = !is_even;
        }
    }

    background->rect.translate(rect.mins);
    background->is_in_bounds = is_in_bounds && ctx->is_hovered(rect);
    background->draw();
    background->rect.translate(rect.mins * -1.f);

    d->add_rect(rect, ctx->colors.outline);

    rect.maxs.y += 10.f;
    post_update();
}

void gui::listbox::post_update() {
    if (!selected_id) {
        if (!elements->elements.empty())
            selected_id = elements->elements.front()->id;
    }
}
