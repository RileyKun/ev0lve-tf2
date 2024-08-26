//
// Created by panzerfaust on 5/30/2020.
//

#include <core/hooks.h>
#include <misc/logger.h>
#include <misc/xor.h>
#include <hacks/esp.h>

struct {
    std::unordered_map<int, int> shots;
} DATA;

void __fastcall hooks::fire_bullets(
        sdk::entity* ecx,
        void*,
        sdk::entity* wep,
        const sdk::fire_bullets_info& info,
        bool do_eff,
        int dmg_type,
        int custom_dmg
) {
    o_fire_bullets(ecx, wep, info, do_eff, dmg_type, custom_dmg);
}