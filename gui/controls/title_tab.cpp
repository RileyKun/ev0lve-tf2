//
// Created by panzerfaust on 5/2/2020.
//
#include "../gui.h"

gui::title_tab::title_tab(rage::joaat_t _id, const std::string& text, const std::string& ico) {
    id      = _id;
    label   = text;
    icon    = ico;

    fill.duration = 0.15f;
    fill.begin  = ctx->colors.text_alt_1;
    fill.last   = ctx->colors.text_alt_1;
    fill.end    = ctx->colors.text_alt_1;

    current_window = nullptr;

    is_active   = true;
    is_visible  = true;

    type = gui_type::TAB_TITLE;
}

void gui::title_tab::update() {
    auto wnd_rect = current_window->rect;
    rect = _RECT(wnd_rect.mins, wnd_rect.mins + renderer::tools::text_size(JOAAT("main"), label));
    rect.translate(_VEC2(65.f, 0.f));

    for (auto& t : current_window->tabs) {
        if (t->id == id)
            break;

        rect.translate(_VEC2(t->rect.width() + 15.f, 0.f));
    }

    rect.maxs.x += renderer::tools::text_size(JOAAT("icons_24"), icon).x;
    rect.maxs.y = rect.mins.y + 50.f;

    if (!is_active || !is_visible || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect)) {
        if (ctx->is_held())
            ctx->active_id = id;
    }

    if (ctx->active_id == id && !ctx->is_held()) {
        ctx->active_id = 0;

        for (auto& t : current_window->tabs) {
            if (t->id == id)
                continue;
            t->is_selected = false;
        }

        is_selected = true;
    }

    auto new_state = 0;
    if (is_selected) {
        fill.end = ctx->colors.accent;
        new_state = 2;
    } else {
        if (ctx->is_hovered(rect)) {
            fill.end = ctx->colors.text_normal;
            new_state = 1;
        } else {
            fill.end = ctx->colors.text_alt_1;
            new_state = 0;
        }
    }

    if (old_state != new_state) {
        fill.reset();
        old_state = new_state;
    }

    ctx->current_id = 0;
}

void gui::title_tab::draw() {
    update();
    if (!is_visible)
        return;

    auto d = &renderer::adapter->background;

    auto icon_size = renderer::tools::text_size(JOAAT("icons_24"), icon);
    d->add_text(JOAAT("icons_24"), icon, rect.mins + _VEC2(0.f, 17.f) + _VEC2(0.f, -(icon_size.y / 2.f)), fill.last);

    d->add_text(is_selected ? JOAAT("bold") : JOAAT("main"), label,
            rect.mins + _VEC2(0.f, 22.f) + _VEC2(icon_size.x + 4.f, -(renderer::tools::text_size(JOAAT("main"), label).y / 2.f)), fill.last);

    if (is_selected) {
        for (auto& e : elements) {
            auto o_rect = e->rect;
            e->rect.translate(current_window->rect.mins);
            e->is_in_bounds = true;
            e->draw();
            e->rect = o_rect;
        }
    }

    post_update();
}

void gui::title_tab::post_update() {
    fill.animate();
}
