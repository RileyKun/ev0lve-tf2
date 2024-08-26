//
// Created by panzerfaust on 6/6/2020.
//

#ifndef EV0LVE_TF_MODEL_H
#define EV0LVE_TF_MODEL_H

namespace sdk {
    class material;

    class brush_data {
    public:
        void* shared;
        int first_model_surface;
        int num_model_surfaces;
        uint16_t render_handle;
        uint16_t first_node;
    };

    class sprite_data {
    public:
        int num_frames;
        int width;
        int height;
        void* sprite;
    };

    class model {
    public:
        void* file_name;
        utl_string name;
        int load_flags;
        int server_cound;
        material** materials;
        int type;
        int flags;
        vec3 mins;
        vec3 maxs;
        float radius;

        union {
            brush_data brush;
            uint16_t model_handle;
            sprite_data sprite;
        };
    };
}

#endif //EV0LVE_TF_MODEL_H
