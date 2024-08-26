//
// Created by panzerfaust on 6/25/2020.
//

#ifndef EV0LVE_TF_ANTIAIM_H
#define EV0LVE_TF_ANTIAIM_H

#include <sdk/sdk.h>

namespace hack {
    enum antiaim_state {
        antiaim_standing,
        antiaim_moving,
        antiaim_air
    };

    class c_antiaim {
    public:
        void work();

    private:
        float get_pitch();
        float get_yaw();

        antiaim_state get_state();
    };

    inline c_antiaim antiaim{};
}

#endif //EV0LVE_TF_ANTIAIM_H
