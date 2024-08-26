//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

gui::small_window::small_window(rage::joaat_t _id, const std::string &title, const types::vec2 &pos,
                                const types::vec2 &sz) {
    id          = _id;
    rect        = _RECT(pos, pos + sz);
    label       = title;

    is_active   = true;
    is_visible  = true;

    cross_color.init(ctx->colors.text_disabled, 0.15f);

    type = gui_type::WINDOW_SMALL;
    callback = []() { return true; };
}

void gui::small_window::update() {
    rect.mins.floor();
    rect.maxs.floor();

    if (is_active) {
        ctx->current_id = id;
        auto cross_rect = _RECT(
                rect.mins + _VEC2(rect.width() - 20.f, 10.f),
                rect.mins + _VEC2(rect.width() - 10.f, 20.f));

        if (ctx->is_hovered(cross_rect)) {
            if (!is_hovered) {
                cross_color.end = ctx->colors.text_normal;
                cross_color.reset();

                is_hovered = true;
            }

            if (ctx->is_clicked() && callback && callback())
                is_visible = false;
        } else {
            if (is_hovered) {
                cross_color.end = ctx->colors.text_disabled;
                cross_color.reset();

                is_hovered = false;
            }
        }
        ctx->current_id = 0;
    }

    cross_color.animate();
}

void gui::small_window::draw() {
    update();
    if (!is_visible)
        return;

    auto d = &renderer::adapter->background;
    d->push_clip_rect(rect);

    // background
    d->add_rect_filled_rounded(_RECT(rect.mins + _VEC2(0.f, 4.f), rect.maxs), ctx->colors.background,3.f, renderer::ROUNDING_B);

    // title bar
    d->push_clip_rect(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 35.f)));
    d->push_texture(ctx->texture);
    d->add_rect_filled_rounded(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 450.f / 800.f * rect.width())), _COLOR(255, 255, 255),
            3.f, renderer::ROUNDING_T);
    d->pop_texture();
    d->pop_clip_rect();

    d->add_rect_filled_rounded(_RECT(rect.mins, rect.mins + _VEC2(4.f, 35.f)), ctx->colors.accent, 3.f, renderer::ROUNDING_TL);
    d->add_line(rect.mins + _VEC2(0.f, 35.f), rect.mins + _VEC2(rect.width(), 35.f), ctx->colors.outline);

    d->add_text(JOAAT("bold"), label, rect.mins + _VEC2(16.f, 4.f), ctx->colors.text_alt_2);

    // cross
    d->add_text(JOAAT("bold"), "x", rect.mins + _VEC2(rect.width() - 20.f, 4.f), cross_color.last);

    ctx->current_window = this;
    for (auto& e : elements) {
        ctx->current_id = id;
        e->is_in_bounds = ctx->is_hovered(rect);
        ctx->current_id = 0;

        if (e->is_layout())
            reinterpret_cast<layout*>(e)->parent = this;
        if (e->type == gui_type::TAB_CONTENT)
            reinterpret_cast<content_tab*>(e)->parent = this;

        e->draw();
    }
    ctx->current_window = nullptr;

    d->pop_clip_rect();

    post_update();
}

void gui::small_window::post_update() {
    if (is_movable && is_active) {
        ctx->current_id = id;
        if (ctx->active_id == id) {
            rect.translate(input::ctx->mouse_delta);
            if (!ctx->is_held())
                ctx->active_id = 0;
        } else {
            if (ctx->is_held() && ctx->is_hovered(rect))
                ctx->active_id = id;
        }
        ctx->current_id = 0;
    }
}
