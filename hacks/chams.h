//
// Created by panzerfaust on 6/1/2020.
//

#ifndef EV0LVE_TF_CHAMS_H
#define EV0LVE_TF_CHAMS_H

#include <ev0lve.h>
#include <sdk/sdk.h>

namespace hack {
    class chams_result {
    public:
        bool should_draw{};
        std::vector<std::tuple<sdk::material*, sdk::color>> data{};
    };

    class c_chams {
    public:
        void init();
        void release();

        chams_result work(sdk::entity* e);

    private:
        sdk::material* select_material(int i, bool is_wall);

        sdk::material* default_open{};
        sdk::material* default_wall{};
        sdk::material* flat_open{};
        sdk::material* flat_wall{};
    };

    inline c_chams chams{};
}

#endif //EV0LVE_TF_CHAMS_H
