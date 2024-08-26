//
// Created by ruppet on 4/13/2020.
//

#ifndef EV0LVE_LOGGER_H
#define EV0LVE_LOGGER_H
#include <ev0lve.h>

namespace logger {
    void create_console();
    void free_console();

    void log(const char* txt, ...);
    void warn(const char* txt, ...);
    void ok(const char* txt, ...);
    void fail(const char* txt, ...);

    inline FILE* out_buffer = nullptr;
}

#endif //EV0LVE_LOGGER_H
