//
// Created by panzerfaust on 5/30/2020.
//

#include <sdk/sdk.h>
#include <misc/xor.h>
#include <misc/mem.h>
#include "entity.h"


void sdk::entity::set_abs_origin(const vec3 &o) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? EB 32 E8 ? ? ? ?"))
            .scan(mem::module(STR("client.dll")))
            .add(1)
            .rip()
            .as<void(__thiscall*)(void*, const vec3&)>();

    fn(this, o);
}

sdk::weapon_data *sdk::entity::get_weapon_data() {
    static auto fn = mem::sig(STR("55 8B EC FF 75 08 E8 ? ? ? ? 83 C4 04 85 C0 75 02"))
            .scan(mem::module(STR("client.dll")))
            .as<weapon_data*(*)(int)>();

    return fn(weapon_id());
}

sdk::vec3 sdk::entity::get_hitbox_pos(sdk::hitbox hb, sdk::matrix3x4* bones) {
    auto mdl = get_model();
    if (!mdl)
        return vec3{};

    auto studio = model_info->get_studio_model(mdl);
    if (!studio)
        return vec3{};

    auto set = studio->hitbox_set(hitbox_set());
    if (!set)
        return vec3{};

    auto box = set->hitbox(hb);
    if (!box)
        return vec3{};

    auto mins = box->bb_min.transform(bones[box->bone]);
    auto maxs = box->bb_max.transform(bones[box->bone]);

    return (mins + maxs) * 0.5f;
}

void sdk::entity::set_abs_angles(const sdk::ang &a) {
    static auto fn = mem::sig(STR("55 8B EC 83 EC 60 56 57 8B F1"))
            .scan(mem::module(STR("client.dll")))
            .as<void(__thiscall*)(void*, const sdk::ang&)>();

    fn(this, a);
}

void sdk::player_anim_state::update(float pitch, float yaw) {
    static auto fn = mem::sig(STR("55 8B EC 81 EC ? ? ? ? 53 57 8B F9 8B 9F ? ? ? ?"))
            .scan(mem::module(STR("client.dll")))
            .as<void(__thiscall*)(void*, float, float)>();

    fn(this, pitch, yaw);
}
