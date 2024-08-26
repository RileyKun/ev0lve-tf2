//
// Created by panzerfaust on 5/2/2020.
//

#include "../gui.h"

gui::layout::layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size) {
    id      = _id;
    rect    = _RECT(pos, pos + size);

    is_active   = true;
    is_visible  = true;

    type = gui_type::LAYOUT;
}

void gui::layout::draw() {
    if (!is_visible)
        return;
    
    update();
    debug::area(rect);

    auto d = &renderer::adapter->background;
    d->push_clip_rect(rect);

    for (auto& e : elements) {
        if (!e->is_visible)
            continue;

        auto o_rect = e->rect;
        e->rect.translate(rect.mins);

        ctx->current_id = id;
        e->is_in_bounds = is_in_bounds && ctx->is_hovered(rect);
        ctx->current_id = 0;

        if (e->is_layout())
            reinterpret_cast<layout*>(e)->parent = this;
        if (e->type == gui_type::TAB_CONTENT)
            reinterpret_cast<content_tab*>(e)->parent = this;

        e->draw();
        e->rect = o_rect;
    }

    d->pop_clip_rect();
    post_update();
}