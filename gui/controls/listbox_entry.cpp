//
// Created by panzerfaust on 5/6/2020.
//

#include "../gui.h"

void gui::listbox_entry::update() {
    if (!is_active || !ctx->current_window->is_active)
        return;

    ctx->current_id = id;

    if (parent->selected_id == id) {
        if (old_state != 2) {
            old_state = 2;

            offset.end = 4.f;
            offset.reset();

            color.end = ctx->colors.accent;
            color.reset();
        }
    } else {
        if (ctx->is_hovered(rect) && is_in_bounds) {
            if (ctx->is_clicked()) {
                parent->selected_id = id;

                if (parent->callback)
                    parent->callback(parent->selected_id);

                ctx->active_id = id;
            }

            if (old_state != 1) {
                old_state = 1;

                offset.end = 0.f;
                offset.reset();

                color.end = ctx->colors.text_normal;
                color.reset();
            }
        } else {
            if (old_state != 0) {
                old_state = 0;

                offset.end = 0.f;
                offset.reset();

                color.end = ctx->colors.text_disabled;
                color.reset();
            }
        }
    }

    if (ctx->is_released() && ctx->active_id == id)
        ctx->active_id = 0;
    ctx->current_id = 0;

    color.animate();
    offset.animate();
}
