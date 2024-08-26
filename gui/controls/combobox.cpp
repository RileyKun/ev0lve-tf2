//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

gui::combobox::combobox(rage::joaat_t _id, const std::vector<std::string>& e, _BITS *v, bool ms, const types::vec2 &size) {
    id = _id;
    elements = e;
    is_multiselect = ms;

    value = v ? v : new _BITS;
    rect = _RECT(_VEC2(40.f, 5.f), size + _VEC2(40.f, 10.f));

    is_visible = true;
    is_active = true;

    open_anim.init(-5.f, 0.1f);
    for (auto i = 0; i < elements.size(); i++) {
        color_anim[i].init(ctx->colors.text_disabled, 0.15f);
        select_anim[i].init(0.f, 0.15f);
    }

    if (!is_multiselect) {
        color_anim[0].end = ctx->colors.accent;
        color_anim[0].reset();

        select_anim[0].end = 4.f;
        select_anim[0].reset();
    }

    hover_col.init(ctx->colors.outline, 0.15f);

    type = gui_type::CONTROL_COMBOBOX;
}

void gui::combobox::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;
    if (ctx->is_hovered(rect)) {
        if (ctx->is_clicked()) {
            if (ctx->active_id != id) {
                ctx->active_id = id;

                open_anim.end = 0.f;
                open_anim.reset();
            }
        }

        if (!hovered) {
            hover_col.end = ctx->colors.text_disabled;
            hover_col.reset();

            hovered = true;
        }
    } else {
        if (hovered) {
            hover_col.end = ctx->colors.outline;
            hover_col.reset();

            hovered = false;
        }
    }

    if (ctx->active_id == id) {
        for (auto i = 0; i < elements.size(); i++) {
            auto is_selected = is_multiselect ? value->get(i) : *value == i;
            auto dr = _RECT(dropdown.mins + _VEC2(0.f, i * 20.f), dropdown.mins + _VEC2(dropdown.width(), i * 20.f + 19.f));

            if (ctx->is_hovered(dr)) {
                if (!ctx->is_unclicked()) {
                    if (!is_hovered[i] && !is_selected) {
                        is_hovered[i] = true;

                        color_anim[i].end = ctx->colors.text_normal;
                        color_anim[i].reset();
                    }
                } else {
                    if (!is_multiselect) {
                        *value = i;

                        for (auto k = 0; k < elements.size(); k++) {
                            color_anim[k].end = k == *value ? ctx->colors.accent : ctx->colors.text_disabled;
                            color_anim[k].reset();

                            select_anim[k].end = k == *value ? 4.f : 0.f;
                            select_anim[k].reset();
                        }
                    } else {
                        value->get(i) ? value->unset(i) : value->set(i);

                        color_anim[i].end = value->get(i) ? ctx->colors.accent : ctx->colors.text_disabled;
                        color_anim[i].reset();

                        select_anim[i].end = value->get(i) ? 4.f : 0.f;
                        select_anim[i].reset();
                    }

                    if (callback)
                        callback(*value);
                }
            } else {
                if (is_hovered[i] && !is_selected) {
                    is_hovered[i] = false;

                    color_anim[i].end = ctx->colors.text_disabled;
                    color_anim[i].reset();
                }
            }
        }

        if (ctx->is_hovered(dropdown) && ctx->is_unclicked()) {
            if (!is_multiselect) {
                ctx->active_id = 0;

                open_anim.end = -5.f;
                open_anim.reset();
            }
        }
    }

    if (!ctx->is_hovered(rect) && !ctx->is_hovered(dropdown) && ctx->is_unclicked() && ctx->active_id == id) {
        ctx->active_id = 0;

        open_anim.end = -5.f;
        open_anim.reset();
    }

    ctx->current_id = 0;
    open_anim.animate();
    hover_col.animate();

    for (auto i = 0; i < elements.size(); i++) {
        color_anim[i].animate();
        select_anim[i].animate();
    }

    dropdown = _RECT(
            rect.mins + _VEC2(0.f, rect.height() + open_anim.last),
            rect.maxs + _VEC2(0.f, elements.size() * 20.f + open_anim.last));
}

void gui::combobox::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 2.f), rect.maxs - _VEC2(-2.f, 5.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), ctx->colors.bottom);

    std::string txt;
    if (is_multiselect) {
        for (auto i = 0; i < elements.size(); i++) {
            if (value->get(i))
                txt += ", " + elements.at(i);
        }

        if (!txt.empty())
            txt = txt.substr(2);
        else
            txt = "None";
    } else
        txt = elements.at(*value);

    d->push_clip_rect(_RECT(rect.mins, rect.maxs - _VEC2(20.f, 0.f)));
    d->add_text(JOAAT("main"), txt, rect.mins + _VEC2(4.f, 0.f), ctx->colors.text_normal);
    d->add_rect_filled_multicolor(_RECT(rect.maxs - _VEC2(60.f, rect.height() - 5.f), rect.maxs - _VEC2(20.f, 7.f)),
            ctx->colors.bottom.mod_a(0.f), ctx->colors.bottom, ctx->colors.bottom, ctx->colors.bottom.mod_a(0.f));
    d->pop_clip_rect();

    if (ctx->active_id == id) {
        d->add_triangle_filled(
                rect.maxs - _VEC2(13.f, rect.height() - 16.f),
                rect.maxs - _VEC2(10.f, rect.height() - 10.f),
                rect.maxs - _VEC2(7.f, rect.height() - 16.f),
                ctx->colors.text_normal);
    } else {
        d->add_triangle_filled(
                rect.maxs - _VEC2(13.f, rect.height() - 12.f),
                rect.maxs - _VEC2(10.f, rect.height() - 18.f),
                rect.maxs - _VEC2(7.f, rect.height() - 12.f),
                ctx->colors.text_normal);
    }

    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 7.f)), hover_col.last);

    if (ctx->active_id == id || open_anim.progress < 1.f) {
        auto _a = ctx->active_id == id ? open_anim.progress : (1.f - open_anim.progress);

        d = &renderer::adapter->foreground;
        d->add_rect_filled(dropdown.expand(2.f), _COLOR(6, 6, 6, 64)
            .mod_a((uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled(dropdown, ctx->colors.bottom.mod_a(_a));

        for (auto i = 0; i < elements.size(); i++) {
            auto is_selected = is_multiselect ? value->get(i) : *value == i;
            auto dr = _RECT(dropdown.mins + _VEC2(0.f, i * 20.f), dropdown.mins + _VEC2(dropdown.width(), i * 20.f + 20.f));

            d->add_rect_filled(dr, i % 2 == 0 ?
                ctx->colors.even.mod_a(_a) :
                ctx->colors.odd.mod_a(_a));
            d->add_rect_filled(_RECT(dr.mins, dr.mins + _VEC2(select_anim[i].last, dr.height())),
                    color_anim[i].last.mod_a(_a));
            d->add_text(is_selected ? JOAAT("bold") : JOAAT("main"), elements.at(i), dr.mins + _VEC2(select_anim[i].last + 4.f, -4.f),
                    color_anim[i].last.mod_a(_a));
        }

        d->add_rect(dropdown, ctx->colors.outline.mod_a(_a));
    }

    post_update();
}
