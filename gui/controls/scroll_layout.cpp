//
// Created by panzerfaust on 5/3/2020.
//

#include "../gui.h"

void gui::scroll_layout::update() {
    if (elements.empty())
        return;

    auto max_y = 0.f;
    for (auto& e : elements) {
        if (!e->is_visible)
            continue;

        auto o_rect = e->rect;
        e->rect.translate(rect.mins);
        if (e->rect.maxs.y > max_y)
            max_y = e->rect.maxs.y;
        e->rect = o_rect;
    }

    need_scroll_bar = max_y > rect.maxs.y;
    temp_size = max_y - rect.maxs.y;

    if (!is_active || !ctx->current_window->is_active)
        return;

    if (need_scroll_bar) {
        scroll_size = std::clamp(rect.height() - temp_size, 20.f, rect.height());

        ctx->current_id = id;
        if (ctx->is_hovered(rect)) {
            if (input::ctx->wheel != 0.f) {
                scroller.end += input::ctx->wheel * 60.f;
                scroller.end = std::clamp(scroller.end, -(temp_size), 0.f);
                scroller.reset();
            }
        }

        auto scroll_bar = _RECT(rect.mins + _VEC2(rect.width() - 4.f, (-scroll / temp_size * rect.height()) - scroll_size * (-scroll / temp_size)),
                                 rect.mins + _VEC2(rect.width(), scroll_size + (-scroll / temp_size * rect.height()) - scroll_size * (-scroll / temp_size)));
        if (ctx->is_hovered(scroll_bar)) {
            if (ctx->is_held())
                ctx->active_id = id;
        }

        if (ctx->active_id == id) {
            if (!ctx->is_held())
                ctx->active_id = 0;

            scroller.last -= input::ctx->mouse_delta.y * (temp_size / rect.height());
            scroller.end = scroller.last;
            scroller.begin = scroller.last;
            scroller.progress = 1.f;
        }
        ctx->current_id = 0;

        scroller.animate();
        scroll = std::clamp(scroller.last, -(temp_size), 0.f);
    }
}

void gui::scroll_layout::draw() {
    if (!is_visible)
        return;
    
    update();
    //debug::area(rect);

    auto d = &renderer::adapter->background;
    d->push_clip_rect(rect);

    for (auto& e : elements) {
        if (!e->is_visible)
            continue;

        e->rect.translate(rect.mins);
        e->rect.translate(_VEC2(0.f, scroll));

        ctx->current_id = id;
        e->is_in_bounds = is_in_bounds && ctx->is_hovered(rect);
        ctx->current_id = 0;

        if (e->is_layout())
            reinterpret_cast<layout*>(e)->parent = this;
        if (e->type == gui_type::TAB_CONTENT)
            reinterpret_cast<content_tab*>(e)->parent = this;

        e->draw();
        e->rect.translate(rect.mins * -1.f);
        e->rect.translate(_VEC2(0.f, -scroll));
    }

    if (need_scroll_bar) {
        d->add_rect_filled(_RECT(
                rect.mins + _VEC2(rect.width() - 2.f, (-scroll / temp_size * rect.height()) - scroll_size * (-scroll / temp_size)),
                rect.mins + _VEC2(rect.width() - 1.f, scroll_size + (-scroll / temp_size * rect.height()) - scroll_size * (-scroll / temp_size))),
                        ctx->colors.accent);
    }

    d->pop_clip_rect();
    post_update();
}

gui::scroll_layout::scroll_layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size) : layout(_id, pos, size) {
    scroller.begin = 0.f;
    scroller.end = 0.f;
    scroller.last = 0.f;
    scroller.duration = 0.15f;

    type = gui_type::LAYOUT_SCROLL;
}
