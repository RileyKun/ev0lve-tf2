//
// Created by panzerfaust on 8/25/2020.
//

#include <misc/mem.h>
#include <misc/xor.h>
#include "game.h"

void game::func::util_traceline(
        const sdk::vec3& start, const sdk::vec3& end, uint32_t mask, sdk::entity* ignore, int cg, sdk::trace* ptr
) {
    static auto fn = mem::sig(STR("53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 6C 8D 4D A0 56 FF 73 0C"))
            .scan(mem::module(STR("client.dll")))
            .as < void(__cdecl*)(const sdk::vec3&, const sdk::vec3&, uint32_t, sdk::entity *, int, sdk::trace *)>();
    if (!fn)
        return;

    fn(start, end, mask, ignore, cg, ptr);
}
