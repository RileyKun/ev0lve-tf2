//
// Created by panzerfaust on 6/1/2020.
//

#ifndef EV0LVE_TF_MODELRENDER_H
#define EV0LVE_TF_MODELRENDER_H

namespace sdk {
    class draw_model_state {
    public:
        studio_hdr* hdr;
        studio_hw_data* hw_data;
        entity_renderable* renderable;
        matrix3x4* model_to_world;
        void* decals;
        int draw_flags;
        int lod;
    };

    class model_render_info {
    public:
        vec3 origin;
        ang angle;
        entity_renderable* renderable;
        model* mdl;
        matrix3x4* model_to_world;
        matrix3x4* lighting_offset;
        vec3* ligting_origin;
        int flags;
        int entity_index;
        int skin;
        int body;
        int hitbox_set;
        uint16_t instance;
    };

    class c_model_render {
    public:
        inline void forced_material_override(material* mat) {
            hack::virtual_func<void(__thiscall*)(void*, material*, int)>(this, 1)(this, mat, 0);
        }
    };

    inline c_model_render* model_render = nullptr;
}

#endif //EV0LVE_TF_MODELRENDER_H
