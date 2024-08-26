//
// Created by panzerfaust on 6/2/2020.
//

#include <core/hooks.h>
#include <hacks/chams.h>
#include <hacks/lc.h>
#include <core/game.h>

void __fastcall hooks::draw_model_execute(
        void* ecx,
        void*,
        const sdk::draw_model_state& state,
        const sdk::model_render_info& info,
        sdk::matrix3x4* mtx
) {
    if (!hack::is_running || !game::me || sdk::studio_render_context->forced_material)
        return o_draw_model_execute(ecx, state, info, mtx);

    auto r = hack::chams.work(sdk::entity_list->get(info.entity_index));
    if (r.should_draw) {
        for (auto& d : r.data) {
            auto mat = std::get<sdk::material*>(d);
            auto col = std::get<sdk::color>(d);

            sdk::render_view->set_color(col);
            sdk::model_render->forced_material_override(mat);

            o_draw_model_execute(ecx, state, info, mtx);

            sdk::render_view->set_color(sdk::color(255, 255, 255, 255));
            sdk::model_render->forced_material_override(nullptr);
        }
    } else
        o_draw_model_execute(ecx, state, info, mtx);
}