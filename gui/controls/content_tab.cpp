//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

gui::content_tab::content_tab(rage::joaat_t _id, const std::string &text) {
    id      = _id;
    label   = text;

    fill.init(ctx->colors.text_alt_1, 0.15f);
    offset.init(0.f, 0.15f);

    is_active   = true;
    is_visible  = true;

    type = gui_type::TAB_CONTENT;
}

void gui::content_tab::update() {
    size = _VEC2(renderer::tools::text_size(JOAAT("main"), label).x + 10.f, 30.f);
    rect.maxs += size;

    std::vector<element*> e_arr;
    if (parent->is_window())
        e_arr = reinterpret_cast<window*>(parent)->elements;
    if (parent->is_tab())
        e_arr = reinterpret_cast<tab*>(parent)->elements;
    if (parent->is_layout())
        e_arr = reinterpret_cast<layout*>(parent)->elements;

    for (auto& e : e_arr) {
        if (e->type == gui_type::TAB_CONTENT) {
            if (e->id == id)
                break;

            rect.translate(_VEC2(reinterpret_cast<content_tab*>(e)->size.x, 0.f));
        }
    }

    if (!is_active || !is_visible || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect)) {
        if (ctx->is_held())
            ctx->active_id = id;
    }
    if (ctx->active_id == id && !ctx->is_held()) {
        ctx->active_id = 0;

        for (auto& e : e_arr) {
            if (e->type == gui_type::TAB_CONTENT) {
                if (e->id == id)
                    continue;
                reinterpret_cast<content_tab*>(e)->is_selected = false;
            }
        }

        is_selected = true;
    }

    auto new_state = 0;
    if (is_selected) {
        fill.end = ctx->colors.accent;
        offset.end = 2.f;
        new_state = 2;
    } else {
        if (ctx->is_hovered(rect)) {
            fill.end = ctx->colors.text_normal;
            offset.end = 0.f;
            new_state = 1;
        } else {
            fill.end = ctx->colors.text_alt_1;
            offset.end = 0.f;
            new_state = 0;
        }
    }

    if (old_state != new_state) {
        fill.reset();
        offset.reset();
        old_state = new_state;
    }
    ctx->current_id = 0;
}

void gui::content_tab::post_update() {
    fill.animate();
    offset.animate();
}

void gui::content_tab::draw() {
    update();
    if (!is_visible)
        return;

    //debug::area(rect);

    auto d = &renderer::adapter->background;
    d->add_text(is_selected ? JOAAT("bold") : JOAAT("main"), label, rect.mins + _VEC2(5.f, 0.f), fill.last);
    d->add_rect_filled(_RECT(rect.maxs - _VEC2(rect.width(), offset.last), rect.maxs), fill.last);

    if (is_selected) {
        for (auto& e : elements) {
            auto o_rect = e->rect;
            e->rect.translate(parent->rect.mins);
            e->is_in_bounds = true;
            e->draw();
            e->rect = o_rect;
        }
    }

    post_update();
}
