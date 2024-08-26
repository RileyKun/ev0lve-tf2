//
// Created by panzerfaust on 6/4/2020.
//

#include <core/hooks.h>
#include <core/cfg.h>

void __fastcall hooks::update(void* ecx, void*) {
    if (!hack::is_running)
        return o_update(ecx);
}