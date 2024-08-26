//
// Created by panzerfaust on 5/30/2020.
//

#ifndef EV0LVE_TF_DEBUG_TOOLS_H
#define EV0LVE_TF_DEBUG_TOOLS_H

#include <ev0lve.h>
#include <types/types.h>

namespace hack {
    class graph {
    public:
        graph(const std::string& name, float min, float max, float rate = 0.1f);

        void draw(const _VEC2& pos);
        void add(float v);

        std::string name;
        float min = 0.f;
        float max = 1.f;
        float rate = 0.f;
        float last_ins_time = 0.f;

        std::vector<float> values;
    };

    class c_debug_tools {
    public:
        void draw();

        std::vector<graph*> graphs;
    };

    inline c_debug_tools debug_tools{};
}

#endif //EV0LVE_TF_DEBUG_TOOLS_H
