//
// Created by panzerfaust on 5/2/2020.
//

#include "../gui.h"

gui::large_window::large_window(rage::joaat_t _id, const _VEC2& pos, const _VEC2& sz) {
    id          = _id;
    rect        = _RECT(pos, pos + sz);

    is_active   = true;
    is_visible  = true;

    cross_color.begin = ctx->colors.text_disabled;
    cross_color.end = ctx->colors.text_disabled;
    cross_color.last = ctx->colors.text_disabled;
    cross_color.duration = 0.15f;

    visible_anim.init(0.f, 0.15f);

    type = gui_type::WINDOW_LARGE;

    callback = []() { return true; };
}

void gui::large_window::update() {
    // prevent anti-alias issues
    rect.mins.floor();
    rect.maxs.floor();

    for (auto& t : tabs) {
        if (t->type == gui_type::TAB_TITLE)
            reinterpret_cast<title_tab*>(t)->current_window = this;
    }

    ctx->current_id = id;
    auto cross_rect = _RECT(
            rect.mins + _VEC2(rect.width() - 30.f, 20.f),
            rect.mins + _VEC2(rect.width() - 20.f, 30.f));

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

    if (is_visible != was_visible) {
        if (is_visible) {
            visible_anim.end = 1.f;
            visible_anim.reset();
        } else {
            visible_anim.end = 0.f;
            visible_anim.reset();
        }

        was_visible = is_visible;
    }

    cross_color.animate();
    visible_anim.animate();
}

void gui::large_window::draw() {
    update();
    if (!is_visible && visible_anim.progress >= 1.f)
        return;

    renderer::adapter->background.push_alpha(visible_anim.last);
    renderer::adapter->foreground.push_alpha(visible_anim.last);

    auto d = &renderer::adapter->background;
    d->push_clip_rect(rect);

    // background
    d->add_rect_filled_rounded(_RECT(rect.mins + _VEC2(0.f, 4.f), rect.maxs), ctx->colors.bottom,3.f, renderer::ROUNDING_B);

    // title bar
    d->push_clip_rect(_RECT(rect.mins + _VEC2(0.f, 4.f), rect.mins + _VEC2(rect.width(), 50.f)));
    d->push_texture(ctx->texture);
    d->add_rect_filled(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 450.f / 800.f * rect.width())), _COLOR(255, 255, 255));
    d->pop_texture();
    d->pop_clip_rect();

    d->add_rect_filled_rounded(_RECT(rect.mins, rect.mins + _VEC2(rect.width(), 4.f)), ctx->colors.accent, 3.f, renderer::ROUNDING_T);
    d->add_line(rect.mins + _VEC2(0.f, 50.f), rect.mins + _VEC2(rect.width(), 50.f), ctx->colors.outline);

    if (!tabs.empty())
        d->add_line(rect.mins + _VEC2(55.f, 12.f), rect.mins + _VEC2(55.f, 46.f), ctx->colors.outline);

    d->add_rect_filled_multicolor(_RECT(rect.mins + _VEC2(0.f, 51.f), rect.mins + _VEC2(rect.width(), 57.f)),
            _COLOR(6, 6, 6, 64),
            _COLOR(6, 6, 6, 64),
            _COLOR(6, 6, 6, 0),
            _COLOR(6, 6, 6, 0));

    // logo
    d->add_text(JOAAT("icons_48"), "H",
            rect.mins + _VEC2(25.f, 27.f) - renderer::tools::text_size(JOAAT("icons_48"), "H") * 0.5f, _COLOR(255, 255, 255));
    d->add_text(JOAAT("icons_86"), "I",
            rect.mins + _VEC2(26.f, 28.f) - renderer::tools::text_size(JOAAT("icons_86"), "I") * 0.5f, ctx->colors.accent);

    // cross
    d->add_text(JOAAT("bold"), "x", rect.mins + _VEC2(rect.width() - 30.f, 14.f), cross_color.last);

    ctx->current_window = this;
    for (auto& t : tabs)
        t->draw();

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

    renderer::adapter->background.pop_alpha();
    renderer::adapter->foreground.pop_alpha();

    post_update();
}

void gui::large_window::post_update() {
    // handle window movement
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

