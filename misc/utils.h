//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_UTILS_H
#define EV0LVE_TF_UTILS_H

#include <ev0lve.h>
#include <types/types.h>
#include <sdk/sdk.h>
#include "mem.h"

namespace utils {
    void show_message(const std::string& title, const std::string& msg);
    void unload(HMODULE mod);
    void wait_for_module(const std::string& mod, uint32_t max_time = 60000);

    std::string format(const char* text, ...);
}

#endif //EV0LVE_TF_UTILS_H
