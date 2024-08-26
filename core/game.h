//
// Created by panzerfaust on 8/25/2020.
//

#ifndef EV0LVE_TF_GAME_H
#define EV0LVE_TF_GAME_H

#include "../sdk/sdk.h"

namespace game {
    inline sdk::user_cmd* cmd = nullptr;
    inline sdk::entity* me = nullptr;

    inline int shift = 0;
    inline bool send_packet = true;

    inline sdk::ang last_angle;

    namespace func {
        void util_traceline(const sdk::vec3& start, const sdk::vec3& end, uint32_t mask, sdk::entity* ignore, int cg, sdk::trace* ptr);
    }
}

#endif //EV0LVE_TF_GAME_H
