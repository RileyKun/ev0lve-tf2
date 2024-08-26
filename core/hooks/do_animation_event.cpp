//
// Created by panzerfaust on 5/31/2020.
//

#include <core/hooks.h>
#include <misc/xor.h>
#include <misc/logger.h>

void __fastcall hooks::do_animation_events(sdk::entity* ecx, void*, int event, int data) {
    o_do_animation_events(ecx, event, data);
}