//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_GLOBAL_VARS_H
#define EV0LVE_TF_GLOBAL_VARS_H

namespace sdk {
    class c_global_vars {
    public:
        float realtime;
        int frame_count;
        float abs_frame_time;
        float server_time;
        float frame_time;
        int max_clients;
        int tick_count;
        float tick_interval;
        float interp_amount;
        int sim_ticks;
        int network_protocol;
        void* save_data;
        bool is_client;
        int networking_base;
        int randomize_window;
    };

    inline c_global_vars* global_vars = nullptr;
}

#endif //EV0LVE_TF_GLOBAL_VARS_H
