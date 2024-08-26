//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_CLIENT_CLASS_H
#define EV0LVE_TF_CLIENT_CLASS_H

namespace sdk {
    class recv_table;

    enum prop_type {
        dpt_int = 0,
        dpt_float,
        dpt_vector,
        dpt_vector2d,
        dpt_string,
        dpt_array,
        dpt_dt,
        dpt_count
    };

    enum send_prop_flags {
        sprop_unsigned			        = 1 << 0,
        sprop_coord				        = 1 << 1,
        sprop_noscale			        = 1 << 2,
        sprop_rounddown			        = 1 << 3,
        sprop_roundup			        = 1 << 4,
        sprop_normal			        = 1 << 5,
        sprop_exclude			        = 1 << 6,
        sprop_xyze				        = 1 << 7,
        sprop_insidearray		        = 1 << 8,
        sprop_proxy_always_yes	        = 1 << 9,
        sprop_changes_often		        = 1 << 10,
        sprop_is_a_vector_elem	        = 1 << 11,
        sprop_collapsible		        = 1 << 12,
        sprop_coord_mp					= 1 << 13,
        sprop_coord_mp_lowprecision 	= 1 << 14,
        sprop_coord_mp_integral			= 1 << 15,
        sprop_varint					= sprop_normal,
        sprop_numflagbits_networked		= 16,
        sprop_encoded_against_tickcount	= 1 << 16,
        sprop_numflagbits				= 17,
    };

    class recv_prop {
    public:
        const char*     name;
        prop_type       type;
        int             flags;
        int             string_size;
        bool            inside_array;
        void*           extra_data;
        recv_prop*      array;
        void*           array_length_proxy_func;
        void*           proxy_func;
        void*           data_table_proxy_func;
        recv_table*     data_table;
        int             offset;
        int             stride;
        int             elements;
        const char*     parent_name;
    };

    class send_prop {
    public:
        void* vtable;
        recv_prop* matching_recv;
        prop_type type;
        int bits;
        float low_value;
        float high_value;
        send_prop* array;
        void* length_proxy;
        int elements;
        int element_stride;
        const char* exclude_dt_name;
        const char* parent_prop_name;
        const char* name;
        float high_low_mul;
        int flags;
        void* proxy;
        void* data_table_proxy;
        void* data_table;
        int offset;
        const void* extra;
    };

    class recv_table {
    public:
        recv_prop*      props;
        int             count;
        void*           decoder;
        const char*     name;
        bool            is_initialized;
        bool            is_in_main_list;
    };

    class client_class {
    public:
        void*           create_func;
        void*           create_event_func;
        const char*     name;
        recv_table*     table;
        client_class*   next;
        int             class_id;
    };
}

#endif //EV0LVE_TF_CLIENT_CLASS_H
