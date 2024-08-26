//
// Created by panzerfaust on 5/25/2020.
//

#include "../gui.h"

gui::color_picker::color_picker(rage::joaat_t _id, types::color *v, bool alpha) {
    id = _id;
    enable_alpha = alpha;

    value = v ? v : new _COLOR(255, 255, 255);
    rect = _RECT(0.f, 5.f, 35.f, 25.f);

    old_value = *value;

    is_visible = true;
    is_active = true;

    open_anim.init(-5.f, 0.1f);
    type = gui_type::CONTROL_COLORPICKER;

    _h = value->h();
    _s = value->s();
    _v = value->v();
}

void gui::color_picker::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    if (old_value != *value) {
        _h = value->h();
        _s = value->s();
        _v = value->v();

        old_value = *value;
    }

    ctx->current_id = id;
    if (ctx->active_id == id) {
        auto sv_rect = _RECT(dropdown.mins + _VEC2(5.f, 5.f), dropdown.mins + _VEC2(145.f, 145.f));
        auto hue_rect = _RECT(dropdown.mins + _VEC2(155.f, 5.f), dropdown.mins + _VEC2(170.f, 145.f));
        auto alpha_rect = _RECT(dropdown.mins + _VEC2(5.f, 155.f), dropdown.mins + _VEC2(175.f, 170.f));

        if (ctx->is_hovered(sv_rect) || changing == 1) {
            if (ctx->is_held() && !changing)
                changing = 1;

            if (changing == 1) {
                _s = std::clamp((input::ctx->mouse.x - sv_rect.mins.x) / sv_rect.width(), 0.f, 1.f);
                _v = 1.f - std::clamp((input::ctx->mouse.y - sv_rect.mins.y) / sv_rect.height(), 0.f, 1.f);
                value->hsv(_h, _s, _v);

                old_value = *value;
                if (callback)
                    callback(*value);
            }
        }

        if (ctx->is_hovered(hue_rect) || changing == 2) {
            if (ctx->is_held() && !changing)
                changing = 2;

            if (changing == 2) {
                _h = std::clamp((input::ctx->mouse.y - hue_rect.mins.y) / hue_rect.height() * 360.f, 0.f, 360.f);
                value->hsv(_h, _s, _v);

                old_value = *value;
                if (callback)
                    callback(*value);
            }
        }

        if (ctx->is_hovered(alpha_rect) || changing == 3) {
            if (ctx->is_held() && enable_alpha && !changing)
                changing = 3;

            if (changing == 3)
                value->a(std::clamp((input::ctx->mouse.x - alpha_rect.mins.x) / alpha_rect.width() * 255.f, 0.f, 255.f));

            old_value = *value;
        }
    }

    if (!ctx->is_hovered(rect) && !ctx->is_hovered(dropdown) && ctx->is_unclicked() && ctx->active_id == id && !changing) {
        ctx->active_id = 0;

        open_anim.end = -5.f;
        open_anim.reset();
    }

    if (!ctx->is_held() && changing)
        changing = 0;

    if (ctx->is_hovered(rect)) {
        if (ctx->is_clicked()) {
            if (ctx->active_id != id) {
                ctx->active_id = id;

                open_anim.end = 0.f;
                open_anim.reset();
            }
        }
    }
    ctx->current_id = 0;

    open_anim.animate();
    dropdown = _RECT(
            rect.mins + _VEC2(0.f, rect.height() + open_anim.last + 5.f),
            rect.mins + _VEC2(180.f, rect.height() + open_anim.last + 155.f + (enable_alpha ? 30.f : 0.f)));
}

void gui::color_picker::draw() {
    if (!is_visible)
        return;

    update();

    auto d = &renderer::adapter->background;
    d->add_rect_filled(_RECT(rect.mins + _VEC2(-2.f, 2.f), rect.maxs - _VEC2(-2.f, 3.f)), _COLOR(6, 6, 6, 64));
    d->add_rect_filled(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 5.f)), *value);
    d->add_rect_filled_multicolor(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 5.f)),
            _COLOR(0, 0, 0, 0), _COLOR(0, 0, 0, 0),
            _COLOR(0, 0, 0, 128), _COLOR(0, 0, 0, 128));
    d->add_rect(_RECT(rect.mins + _VEC2(0.f, 5.f), rect.maxs - _VEC2(0.f, 5.f)), ctx->colors.outline);

    if (ctx->active_id == id || std::clamp(open_anim.progress, 0.f, 1.f) < 1.f) {
        auto _a = ctx->active_id == id ?
                std::clamp(open_anim.progress, 0.f, 1.f) :
                (1.f - std::clamp(open_anim.progress, 0.f, 1.f));

        static std::vector<_COLOR> hues {
            _COLOR(255, 0, 0),
            _COLOR(255, 255, 0),
            _COLOR(0, 255, 0),
            _COLOR(0, 255, 255),
            _COLOR(0, 0, 255),
            _COLOR(255, 0, 255),
            _COLOR(255, 0, 0)
        };

        d = &renderer::adapter->foreground;
        d->add_rect_filled(dropdown.expand(2.f), _COLOR(6, 6, 6, 64)
                .mod_a((uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled(dropdown, ctx->colors.bottom.mod_a(_a));

        auto sv_rect = _RECT(dropdown.mins + _VEC2(5.f, 5.f), dropdown.mins + _VEC2(145.f, 145.f));
        d->add_rect_filled(sv_rect.expand(2.f), _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled_multicolor(sv_rect,
                _COLOR(1.f, 1.f, 1.f, _a),
                _COLOR().hsv(_h, 1.f, 1.f, _a),
                _COLOR().hsv(_h, 1.f, 1.f, _a),
                _COLOR(1.f, 1.f, 1.f, _a));
        d->add_rect_filled_multicolor(sv_rect,
                _COLOR(0.f, 0.f, 0.f, 0.f),
                _COLOR(0.f, 0.f, 0.f, 0.f),
                _COLOR(0.f, 0.f, 0.f, 1.f),
                _COLOR(0.f, 0.f, 0.f, 1.f));
        d->add_rect(sv_rect, ctx->colors.outline.mod_a(_a));

        auto sv_thumb = sv_rect.mins + _VEC2(_s * sv_rect.width(), (1.f - _v) * sv_rect.height());
        d->add_rect_filled(_RECT(sv_thumb, sv_thumb).expand(2.f),
                _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled(_RECT(sv_thumb, sv_thumb).expand(1.f),
                ctx->colors.text_normal.mod_a(_a));

        auto hue_rect = _RECT(dropdown.mins + _VEC2(155.f, 5.f), dropdown.mins + _VEC2(170.f, 145.f));
        d->add_rect_filled(hue_rect.expand(2.f), _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));

        for (auto i = 0; i < hues.size() - 1; i++) {
            auto chunk_size = hue_rect.height() / (float)(hues.size() - 1);
            d->add_rect_filled_multicolor(
                    _RECT(
                            hue_rect.mins + _VEC2(0.f, chunk_size * i),
                            hue_rect.mins + _VEC2(hue_rect.width(), chunk_size * i + chunk_size)),
                    hues.at(i).mod_a(_a), hues.at(i).mod_a(_a),
                    hues.at(i + 1).mod_a(_a), hues.at(i + 1).mod_a(_a));
        }
        d->add_rect(hue_rect, ctx->colors.outline.mod_a(_a));

        auto h_thumb = hue_rect.mins + _VEC2(0.f, _h / 360.f * hue_rect.height());
        d->add_rect_filled(_RECT(h_thumb, h_thumb + _VEC2(hue_rect.width(), 0.f)).expand(2.f),
                _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
        d->add_rect_filled(_RECT(h_thumb, h_thumb + _VEC2(hue_rect.width(), 0.f)).expand(1.f),
                ctx->colors.text_normal.mod_a(_a));

        if (enable_alpha) {
            auto alpha_rect = _RECT(dropdown.mins + _VEC2(5.f, 155.f), dropdown.mins + _VEC2(175.f, 170.f));
            d->add_rect_filled(alpha_rect.expand(2.f), _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
            d->add_rect_filled_multicolor(alpha_rect, value->mod_a(0.f), value->mod_a(_a), value->mod_a(_a), value->mod_a(0.f));
            d->add_rect(alpha_rect, ctx->colors.outline.mod_a(_a));

            auto a_thumb = alpha_rect.mins + _VEC2((float)value->a() / 255.f * alpha_rect.width(), 0.f);
            d->add_rect_filled(_RECT(a_thumb, a_thumb + _VEC2(0.f, alpha_rect.height())).expand(2.f),
                               _COLOR(6, 6, 6, (uint32_t)std::clamp((int)(_a * 255.f), 0, 64)));
            d->add_rect_filled(_RECT(a_thumb, a_thumb + _VEC2(0.f, alpha_rect.height())).expand(1.f),
                               ctx->colors.text_normal.mod_a(_a));
        }

        d->add_rect(dropdown, ctx->colors.outline.mod_a(_a));
    }

    post_update();
}
