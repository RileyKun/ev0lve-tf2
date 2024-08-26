//
// Created by panzerfaust on 6/4/2020.
//

#include <core/hooks.h>
#include <core/cfg.h>
#include <sdk/sdk.h>
#include <core/game.h>

void __fastcall hooks::override_view(void* ecx, void*, sdk::view_setup* setup) {
    if (!hack::is_running || !game::me)
        return o_override_view(ecx, setup);

    setup->fov += _CFG("misc_fov").get<float>(0.f);

    if (sdk::input->is_third_person() &&
        game::me->life_state() == sdk::ls_alive &&
        !game::me->force_taunt_cam()) {

        sdk::ang va;
        sdk::engine->get_view_angles(va);

        sdk::trace tr{};
        sdk::vec3 cam_forward, cam_right, cam_up;
        va.vectors(cam_forward, cam_right, cam_up);

        auto origin = game::me->get_abs_origin();
        origin.z += game::me->view_offset().z;

        sdk::ray r{};
        r.init(
                origin,
                origin + (cam_forward * -_CFG("vis_other_tp_dist").get<float>(150.f)),
                sdk::vec3(-14.f, -14.f, -14.f),
                sdk::vec3(14.f, 14.f, 14.f)
        );

        sdk::trace_filter_world_only f{};
        sdk::engine_trace->trace_ray(r, sdk::mask_solid & ~sdk::contents_monster, &f, &tr);

        setup->origin = tr.end;
    }

    o_override_view(ecx, setup);
}