//
// Created by panzerfaust on 6/4/2020.
//

#ifndef EV0LVE_TF_STUDIO_RENDER_CONTEXT_H
#define EV0LVE_TF_STUDIO_RENDER_CONTEXT_H

namespace sdk {
    class c_studio_render_context {
    public:
        void *vtable; //0x0000
        bool is_primary_app_system; //0x0004
        char pad_0005[3]; //0x0005
        float eye_shift_x; //0x0008
        float eye_shift_y; //0x000C
        float eye_shift_z; //0x0010
        float eye_size; //0x0014
        float eye_lod_threshold; //0x0018
        int32_t max_decals_per_model; //0x001C
        int32_t draw_entities; //0x0020
        int32_t skin; //0x0024
        int32_t fullbright; //0x0028
        char pad_002C[552]; //0x002C
        void *forced_material; //0x0254
    };

    inline c_studio_render_context* studio_render_context = nullptr;
}

#endif //EV0LVE_TF_STUDIO_RENDER_CONTEXT_H
