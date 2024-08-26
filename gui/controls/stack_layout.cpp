//
// Created by panzerfaust on 5/3/2020.
//

#ifndef RENDERER_TEST_STACK_LAYOUT_CPP
#define RENDERER_TEST_STACK_LAYOUT_CPP

#include "../gui.h"

gui::stack_layout::stack_layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size, gui::stack_direction dir)
    : layout(_id, pos, size) {
    direction = dir;

    is_active   = true;
    is_visible  = true;

    type = gui_type::LAYOUT_STACK;
}

void gui::stack_layout::draw() {
    if (!is_visible)
        return;
    
    update();
    //debug::area(rect);

    auto d = &renderer::adapter->background;
    d->push_clip_rect(rect);

    auto stack = _VEC2(0.f, 0.f);
    for (auto& e : elements) {
        if (!e->is_visible)
            continue;

        e->rect.translate(rect.mins);
        e->rect.translate(stack);

        if (stack != _VEC2(0.f, 0.f) && direction == stack_direction::HORIZONTAL)
            e->rect.translate(_VEC2(-e->rect.width(), 0.f));

        ctx->current_id = id;
        e->is_in_bounds = is_in_bounds && ctx->is_hovered(rect);
        ctx->current_id = 0;

        if (e->is_layout())
            reinterpret_cast<layout*>(e)->parent = this;
        if (e->type == gui_type::TAB_CONTENT)
            reinterpret_cast<content_tab*>(e)->parent = this;

        e->draw();
        e->rect.translate(rect.mins * -1.f);
        e->rect.translate(stack * -1.f);

        if (stack != _VEC2(0.f, 0.f) && direction == stack_direction::HORIZONTAL)
            e->rect.translate(_VEC2(e->rect.width(), 0.f));

        if (direction == stack_direction::VERTICAL)
            stack.y += e->rect.height();
        else {
            if (parent) {
                if (stack != _VEC2(0.f, 0.f))
                    stack.x -= e->rect.width() + 5.f;
                else
                    stack.x = parent->rect.width() - 10.f;
            }
        }
    }

    d->pop_clip_rect();
    post_update();
}

void gui::stack_layout::update() {
    if (direction == stack_direction::VERTICAL) {
        rect.maxs.y = rect.mins.y;
        for (auto& e : elements)
            rect.maxs.y += e->rect.height();
        rect.maxs.y += 2.f;
    } else {
        if (!parent)
            return;

        rect.maxs.x = parent->rect.maxs.x;

        auto max_h = 0;
        for (auto& e : elements) {
            if (e->rect.height() > max_h)
                max_h = e->rect.height();
        }

        rect.maxs.y = rect.mins.y + max_h;
        if (rect.height() < 25.f)
            rect.maxs.y = rect.mins.y + 25.f;
    }

    if (!is_active || !ctx->current_window->is_active)
        return;
}


#endif //RENDERER_TEST_STACK_LAYOUT_CPP
