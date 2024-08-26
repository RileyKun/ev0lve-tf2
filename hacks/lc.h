//
// Created by panzerfaust on 6/7/2020.
//

#ifndef EV0LVE_TF_LC_H
#define EV0LVE_TF_LC_H

#include <sdk/sdk.h>

namespace hack {
    class lc_record {
    public:
        sdk::entity* entity{};
        sdk::vec3 pos;
        float sim_time{};
        sdk::matrix3x4 bones[128]{};
        int id{};
    };

    class c_lc {
    public:
        void work();

        lc_record get_last(sdk::entity* ent);
        lc_record get_first(sdk::entity* ent);
        std::vector<lc_record>& get(sdk::entity* ent);

    private:
        void populate(sdk::entity* ent);
        bool is_valid(lc_record& r);
        float get_lerp_time();

        std::unordered_map<uint32_t, std::vector<lc_record>> records;
    };

    inline c_lc lc{};
}

#endif //EV0LVE_TF_LC_H
