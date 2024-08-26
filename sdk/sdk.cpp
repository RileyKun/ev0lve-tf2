//
// Created by panzerfaust on 5/28/2020.
//

#include <sdk/sdk.h>
#include <misc/xor.h>
#include <misc/mem.h>
#include "keyvalues.h"


void sdk::user_cmd::fix_movement(ang &wish) {
    vec3 view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
    wish.vectors(view_fwd, view_right, view_up);
    view_angles.vectors(cmd_fwd, cmd_right, cmd_up);

    view_fwd.z = view_right.z = 0.f;
    cmd_fwd.z = cmd_right.z = 0.f;
    view_up = cmd_up = { 0.f, 0.f, 1.f };

    view_fwd.normalize();
    view_right.normalize();
    cmd_fwd.normalize();
    cmd_right.normalize();

    view_fwd *= forward_move;
    view_right *= side_move;
    view_up *= up_move;

    forward_move = cmd_fwd.dot(view_fwd) + cmd_fwd.dot(view_right) + cmd_fwd.dot(view_up);
    side_move = cmd_right.dot(view_fwd) + cmd_right.dot(view_right) + cmd_right.dot(view_up);
    up_move = cmd_up.dot(view_fwd) + cmd_up.dot(view_right) + cmd_up.dot(view_up);

    forward_move = std::clamp(forward_move, -450.f, 450.f);
    side_move = std::clamp(side_move, -450.f, 450.f);
    up_move = std::clamp(up_move, -450.f, 450.f);

    wish = view_angles;
}

void sdk::key_values::set_name(const std::string &name) {
    static auto key_system = mem::module(STR("vstdlib.dll"))
            .get_export(STR("KeyValuesSystem"))
            .as<void*(*)()>();
    static auto fn = mem::sig(STR("55 8B EC 83 EC 24 53 56 8B 75 08"))
            .scan(mem::module(STR("vstdlib.dll")))
            .as<int(__thiscall*)(void*, const char*, bool)>();

    if (!key_system || !fn)
        return;

    key_name = fn(key_system(), name.c_str(), true);
}

void sdk::key_values::set_string(const std::string &key, const std::string &value) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? DD 45 D0"))
                             .scan(mem::module(STR("engine.dll")))
                             .add(1)
                             .rip()
                             .as<void(__thiscall*)(void*, const char*, const char*)>();

    if (!fn)
        return;

    fn(this, key.c_str(), value.c_str());
}

void sdk::key_values::set_int(const std::string &key, int value) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? D9 47 30"))
                             .scan(mem::module(STR("engine.dll")))
                             .add(1)
                             .rip()
                             .as<void(__thiscall*)(void*, const char*, int)>();

    if (!fn)
        return;

    fn(this, key.c_str(), value);
}

void sdk::key_values::set_float(const std::string &key, float value) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? D9 47 24"))
                             .scan(mem::module(STR("engine.dll")))
                             .add(1)
                             .rip()
                             .as<void(__thiscall*)(void*, const char*, float)>();

    if (!fn)
        return;

    fn(this, key.c_str(), value);
}

void sdk::key_values::set_color(const std::string &key, sdk::color value) {
    static auto fn = mem::sig(STR("E8 ? ? ? ? D9 46 40"))
                             .scan(mem::module(STR("client.dll")))
                             .add(1)
                             .rip()
                             .as<void(__thiscall*)(void*, const char*, sdk::color)>();

    if (!fn)
        return;

    fn(this, key.c_str(), value);
}