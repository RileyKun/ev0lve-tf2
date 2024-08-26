//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_SURFACE_H
#define EV0LVE_TF_SURFACE_H

#include <types/types.h>

namespace sdk {
    class c_surface {
    public:
        inline void unlock_cursor() {
            hack::virtual_func<void(__thiscall*)(void*)>(this, hack::APP_SYSTEM_M + 56)(this);
        }
    };

    inline c_surface* surface = nullptr;
}

#endif //EV0LVE_TF_SURFACE_H
