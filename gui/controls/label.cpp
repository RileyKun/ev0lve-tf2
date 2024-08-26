//
// Created by panzerfaust on 5/4/2020.
//

#include "../gui.h"

gui::text_label::text_label(rage::joaat_t _id, const std::string &text, bool bold, const _COLOR& _color) {
    id = _id;
    label = text;
    is_bold = bold;
    
    is_visible = true;
    is_active = true;

    if (!(uint32_t)_color)
        color = ctx->colors.text_normal;
    else
        color = _color;
    
    rect = _RECT(_VEC2(10.f, 5.f), _VEC2(0.f, 0.f));

    type = gui_type::CONTROL_LABEL;
}

void gui::text_label::update() {
    if (!set_rect) {
        rect.maxs = rect.mins + _VEC2(40.f + renderer::tools::text_size(is_bold ? JOAAT("bold") : JOAAT("main"), label).x, 20.f);
        set_rect = true;
    }

    if (!is_active || !ctx->current_window->is_active)
        return;
}

void gui::text_label::draw() {
    update();

    renderer::adapter->background.add_text(is_bold ? JOAAT("bold") : JOAAT("main"),
            label, rect.mins + _VEC2(30.f, -4.f), color);

    post_update();
}
