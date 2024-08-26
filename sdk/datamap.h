//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_DATAMAP_H
#define EV0LVE_TF_DATAMAP_H

namespace sdk {
    enum field_type {
        field_void = 0,
        field_float,
        field_string,
        field_vector,
        field_quaternion,
        field_integer,
        field_boolean,
        field_short,
        field_character,
        field_color32,
        field_embedded,
        field_custom,
        field_class_ptr,
        field_e_handle,
        field_edict,
        field_position_vector,
        field_time,
        field_tick,
        field_model_name,
        field_sound_name,
        field_input,
        field_function,
        field_v_matrix,
        field_v_matrix_world_space,
        field_matrix3x4_world_space,
        field_interval,
        field_model_index,
        field_material_index,
        field_vector2d,
        field_type_count,
    };

    class data_map;

    class type_desc {
    public:
        field_type      type;
        const char*     name;
        int             offset[2];
        uint16_t        size;
        short           flags;
        const char*     ext_name;
        void*           save_restore_ops;
        void*           input_func;
        data_map*       embed;
        int             member_size;
        type_desc*      override;
        int             override_count;
        float           tolerance;
    };

    class data_map {
    public:
        type_desc*      desc;
        int             fields;
        const char*     name;
        data_map*       parent;
        bool            validated;
        bool            packed;
        int             size;
    };
}

#endif //EV0LVE_TF_DATAMAP_H
