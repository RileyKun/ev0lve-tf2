//
// Created by panzerfaust on 6/7/2020.
//

#ifndef EV0LVE_TF_AIMBOT_H
#define EV0LVE_TF_AIMBOT_H

#include <sdk/sdk.h>

namespace hack {
    class aim_target {
    public:
        bool can_hit{};
        sdk::entity* entity{};
        sdk::vec3 pos{};
    };

    class aim_bullet {
    public:
        sdk::vec3 pos{};
        bool is_multipoint{};
        int hitgroup{};
    };

    class c_aimbot {
    public:
        void work();

        bool is_enabled = false;

    private:
        aim_target get_target();
        bool can_hit(sdk::entity* e, aim_target& b);
        void aim(aim_target& b);
    };

    inline c_aimbot aimbot{};
}

#endif //EV0LVE_TF_AIMBOT_H
