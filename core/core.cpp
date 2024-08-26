//
// Created by panzerfaust on 5/25/2020.
//
#include <ev0lve.h>
#include <core/core.h>
#include <core/hooks.h>
#include <misc/utils.h>
#include <misc/logger.h>
#include <misc/mem.h>
#include <misc/xor.h>
#include <sdk/sdk.h>

#include <MinHook.h>
#include <hacks/game_events.h>
#include <hacks/chams.h>
#include "game.h"

namespace core {
    template<typename T>
    inline T* create_interface(const std::string& mod, const std::string& i) {
        return reinterpret_cast<T*>(mem::module(mod.c_str())
            .get_export(STR("CreateInterface"))
            .as<void*(*)(const char*, int)>()(i.c_str(), 0));
    }

    std::unordered_map<uint32_t, sdk::netvar> dump_recv_table(
            sdk::recv_table* t,
            std::optional<sdk::netvar> parent = std::nullopt
    ) {
        std::unordered_map<uint32_t, sdk::netvar> ret;

        for (auto i = 0; i < t->count; i++) {
            auto p = &t->props[i];
            auto child = p->data_table;

            auto n = sdk::netvar {
                p->name,
                p->offset + parent.value_or(sdk::netvar{}).offset,
                p->type
            };

            if (child)
                n.children = dump_recv_table(child, n);

            ret[rage::joaat(p->name)] = n;
        }

        return ret;
    }

    void dump_netvars(const std::unordered_map<uint32_t, sdk::netvar>& children, std::string& text, uint32_t offset = 0) {
        for (auto& c : children) {
            for (auto i = 0; i < offset; i++)
                text += STR("\t");

            text += utils::format(STR("[%08X] %s (type = "), c.first, c.second.name.c_str());
            switch (c.second.type) {
                case sdk::dpt_int:
                    text += STR("int");
                    break;
                case sdk::dpt_float:
                    text += STR("float");
                    break;
                case sdk::dpt_vector:
                    text += STR("vec3");
                    break;
                case sdk::dpt_vector2d:
                    text += STR("vec2");
                    break;
                case sdk::dpt_string:
                    text += STR("string");
                    break;
                case sdk::dpt_array:
                    text += STR("array");
                    break;
                case sdk::dpt_dt:
                    text += STR("dt");
                    break;
                case sdk::dpt_count:
                    text += STR("invalid");
                    break;
            }

            text += utils::format(STR(", offset = %i"), c.second.offset);
            if (!c.second.children.empty())
                text += utils::format(STR(", children = %i): \n"), c.second.children.size());
            else
                text += STR(") \n");

            dump_netvars(c.second.children, text, offset + 1);
        }
    }

    void dump_netvars() {
        std::string text;

        for (auto& t : sdk::netvars) {
            if (t.second.children.empty())
                continue;

            text += utils::format(STR("[%08X] %s (children = %i): \n"), t.first, t.second.name.c_str(), t.second.children.size());
            dump_netvars(t.second.children, text, 1);

            logger::log(STR("Dumper: dumped %s"), t.second.name.c_str());
        }

        std::ofstream(STR("TF2_NETVARS.txt")) << text;
        logger::ok(STR("Dumper: all netvars have been dumped to file"));
    }
}

bool core::init() {
    logger::log(STR("Init started"));

    if (FAILED(MH_Initialize()))
        return false;

    if (!load_interfaces())
        return false;
    if (!load_netvars())
        return false;
    if (!hook_game())
        return false;
    if (!hook_render())
        return false;
    if (!hook_window())
        return false;
    if (!patch_game())
        return false;

    hack::game_events.init();
    hack::chams.init();

    logger::ok(STR("Init done"));
    return true;
}

bool core::hook_render() {
    auto present_addr = mem::sig(STR("FF 15 ? ? ? ? 8B F8 85 DB"))
            .scan(mem::module(STR("gameoverlayrenderer.dll")))
            .add(2);
    auto reset_addr = mem::sig(STR("FF 15 ? ? ? ? 8B F8 85 FF 78 18"))
            .scan(mem::module(STR("gameoverlayrenderer.dll")))
            .add(2);

    if (!present_addr || !reset_addr)
        return false;

    DATA.present = (void*)***present_addr.as<hooks::fnPresent **>();
    DATA.reset = (void*)***reset_addr.as<hooks::fnReset **>();

    if (FAILED(MH_CreateHook(DATA.present, (void*)&hooks::present, (void**)&hooks::o_present)))
        return false;
    if (FAILED(MH_CreateHook(DATA.reset, (void*)&hooks::reset, (void**)&hooks::o_reset)))
        return false;

    if (FAILED(MH_EnableHook(DATA.present)))
        return false;
    if (FAILED(MH_EnableHook(DATA.reset)))
        return false;

    return true;
}

void core::shutdown() {
    if (sdk::input && sdk::engine && sdk::engine->is_in_game() && sdk::input->is_third_person()) {
        sdk::input->to_first_person();
        if (game::me)
            game::me->thirdperson_switch(false);
    }

    hack::game_events.release();

    unhook_game();
    unhook_render();
    unhook_window();
    unpatch_game();

    MH_Uninitialize();
}

void core::unhook_render() {
    MH_DisableHook(DATA.present);
    MH_DisableHook(DATA.reset);

    MH_RemoveHook(DATA.present);
    MH_RemoveHook(DATA.reset);
}

bool core::hook_window() {
    hooks::o_wndproc = (WNDPROC)SetWindowLongA(FindWindowA(STR("Valve001"), nullptr), GWL_WNDPROC, (long)&hooks::wnd_proc);
    return true;
}

void core::unhook_window() {
    SetWindowLongA(FindWindowA(STR("Valve001"), nullptr), GWL_WNDPROC, (long)hooks::o_wndproc);
}

bool core::load_interfaces() {
    {
        sdk::global_vars = mem::sig(STR("56 7E 12"))
                .scan(mem::module(STR("engine.dll")))
                .sub(5)
                .unref()
                .sub(40)
                .as<sdk::c_global_vars *>();
        if (!sdk::global_vars)
            return false;
        logger::ok(STR("Found CGlobalVarsBase at 0x%p"), sdk::global_vars);
    }
    {
        sdk::client = create_interface<sdk::c_client>(STR("client.dll"), STR("VClient017"));
        if (!sdk::client)
            return false;
        logger::ok(STR("Found IBaseClientDLL at 0x%p"), sdk::client);
    }
    {
        sdk::surface = create_interface<sdk::c_surface>(STR("vguimatsurface.dll"), STR("VGUI_Surface030"));
        if (!sdk::surface)
            return false;
        logger::ok(STR("Found ISurface at 0x%p"), sdk::surface);
    }
    {
        sdk::engine = create_interface<sdk::c_engine>(STR("engine.dll"), STR("VEngineClient013"));
        if (!sdk::engine)
            return false;
        logger::ok(STR("Found IVEngineClient at 0x%p"), sdk::engine);
    }
    {
        sdk::entity_list = create_interface<sdk::c_entity_list>(STR("client.dll"), STR("VClientEntityList003"));
        if (!sdk::entity_list)
            return false;
        logger::ok(STR("Found IClientEntityList at 0x%p"), sdk::entity_list);
    }
    {
        sdk::input = mem::sig(STR("FF 75 08 FF 50 0C 8B 06"))
                .scan(mem::module(STR("client.dll")))
                .sub(16)
                .unref()
                .unref()
                .as<sdk::c_input*>();
        if (!sdk::input)
            return false;
        logger::ok(STR("Found CTFInput at 0x%p"), sdk::input);
    }
    {
        sdk::client_state = mem::sig(STR("55 8B EC 83 EC 38 83 3D ? ? ? ? ?"))
                .scan(mem::module(STR("engine.dll")))
                .add(8)
                .unref()
                .sub(296)
                .as<sdk::c_client_state*>();
        if (!sdk::client_state)
            return false;
        logger::ok(STR("Found CClientState at 0x%p"), sdk::client_state);
    }
    {
        sdk::client_mode = mem::sig(STR("FF 50 20 50 8B CB E8 ? ? ? ? 68 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 04 85 C0 74 11 6A 01 68 ? ? ? ? 53 8B C8 E8 ? ? ? ? EB 02 33 C0 68 ? ? ? ?"))
                .scan(mem::module(STR("client.dll")))
                .sub(6)
                .unref()
                .unref()
                .as<sdk::c_client_mode*>();
        if (!sdk::client_mode)
            return false;
        logger::ok(STR("Found IClientMode at 0x%p"), sdk::client_mode);
    }
    {
        sdk::cvar = mem::sig(STR("E8 ? ? ? ? 84 C0 75 9D"))
                .scan(mem::module(STR("engine.dll")))
                .add(1)
                .rip()
                .add(0x2E)
                .add(2)
                .unref()
                .unref()
                .as<sdk::c_cvar*>();
        if (!sdk::cvar)
            return false;
        logger::ok(STR("Found ICVar at 0x%p"), sdk::cvar);
    }
    {
        sdk::debug_overlay = create_interface<sdk::c_debug_overlay>(STR("engine.dll"), STR("VDebugOverlay003"));
        if (!sdk::debug_overlay)
            return false;
        logger::ok(STR("Found IVDebugOverlay at 0x%p"), sdk::debug_overlay);
    }
    {
        sdk::localize = mem::sig(STR("FF 52 08 50 8B CF FF 96 ? ? ? ? 8B 07 8B CF 6A 00 6A 01 FF 90 ? ? ? ? 8B 75 D0"))
                .scan(mem::module(STR("client.dll")))
                .sub(17)
                .unref()
                .unref()
                .as<sdk::c_localize*>();
        if (!sdk::localize)
            return false;
        logger::ok(STR("Found ILocalize at 0x%p"), sdk::localize);
    }
    {
        sdk::event_manager = create_interface<sdk::c_game_event_manager>(STR("engine.dll"), STR("GAMEEVENTSMANAGER002"));
        if (!sdk::event_manager)
            return false;
        logger::ok(STR("Found IGameEventsManager2 at 0x%p"), sdk::event_manager);
    }
    {
        sdk::material_system = mem::sig(STR("52 FF 90 ? ? ? ? 6A 40"))
                .scan(mem::module(STR("engine.dll")))
                .sub(0xF)
                .unref()
                .unref()
                .as<sdk::c_material_system*>();
        if (!sdk::material_system)
            return false;
        logger::ok(STR("Found IMaterialSystem at 0x%p"), sdk::material_system);
    }
    {
        sdk::model_render = create_interface<sdk::c_model_render>(STR("engine.dll"), STR("VEngineModel016"));
        if (!sdk::model_render)
            return false;
        logger::ok(STR("Found IVModelRender at 0x%p"), sdk::model_render);
    }
    {
        sdk::render_view = create_interface<sdk::c_render_view>(STR("engine.dll"), STR("VEngineRenderView014"));
        if (!sdk::render_view)
            return false;
        logger::ok(STR("Found IVRenderView at 0x%p"), sdk::render_view);
    }
    {
        sdk::studio_render_context = mem::sig(STR("FF 50 6C 8B 0D ? ? ? ? D9 05 ? ? ? ? 51 D9 1C 24 8B 01 FF 50 70 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ?"))
                .scan(mem::module(STR("engine.dll")))
                .add(5)
                .unref()
                .unref()
                .as<sdk::c_studio_render_context*>();
        if (!sdk::studio_render_context)
            return false;
        logger::ok(STR("Found IStudioRender at 0x%p"), sdk::studio_render_context);
    }
    {
        sdk::engine_trace = create_interface<sdk::c_engine_trace>(STR("engine.dll"), STR("EngineTraceClient003"));
        if (!sdk::engine_trace)
            return false;
        logger::ok(STR("Found IEngineTrace at 0x%p"), sdk::engine_trace);
    }
    {
        sdk::model_info = create_interface<sdk::c_model_info>(STR("engine.dll"), STR("VModelInfoClient006"));
        if (!sdk::model_info)
            return false;
        logger::ok(STR("Found IVModelInfo at 0x%p"), sdk::model_info);
    }
    {
        sdk::game_movement = create_interface<sdk::c_game_movement>(STR("client.dll"), STR("GameMovement001"));
        if (!sdk::game_movement)
            return false;
        logger::ok(STR("Found IGameMovement at 0x%p"), sdk::game_movement);
    }
    {
        sdk::move_helper = mem::sig(STR("FF 56 14 5E C3"))
                .scan(mem::module(STR("client.dll")))
                .sub(23)
                .unref()
                .unref()
                .as<sdk::c_move_helper*>();
        if (!sdk::move_helper)
            return false;
        logger::ok(STR("Found IMoveHelper at 0x%p"), sdk::move_helper);
    }
    {
        sdk::game_prediction = create_interface<sdk::c_game_prediction>(STR("client.dll"), STR("VClientPrediction001"));
        if (!sdk::game_prediction)
            return false;
        logger::ok(STR("Found CTFPrediction at 0x%p"), sdk::game_prediction);
    }
    {
        sdk::thirdperson_manager = mem::sig(STR("B9 ? ? ? ? E8 ? ? ? ? 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 3D ? ? ? ? ?"))
                .scan(mem::module(STR("client.dll")))
                .add(1)
                .unref()
                .as<sdk::c_thirdperson_manager*>();
        if (!sdk::thirdperson_manager)
            return false;
        logger::ok(STR("Found CThirdPersonManager at 0x%p"), sdk::thirdperson_manager);
    }

    return true;
}

bool core::hook_game() {
    DATA.send_move = mem::sig(STR("E8 ? ? ? ? 80 7D FF 00 0F 84 ? ? ? ?"))
            .scan(mem::module(STR("engine.dll")))
            .add(1)
            .rip()
            .as<void*>();
    DATA.fire_bullets = mem::sig(STR("E8 ? ? ? ? 8B 45 20 47"))
            .scan(mem::module(STR("client.dll")))
            .add(1)
            .rip()
            .as<void*>();
    DATA.update = mem::sig(STR("E8 ? ? ? ? 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 3D ? ? ? ? ?"))
            .scan(mem::module(STR("client.dll")))
            .add(1)
            .rip()
            .as<void*>();
    DATA.encode_float = mem::sig(STR("55 8B EC 8B 45 08 83 EC 10 8B 50 38"))
            .scan(mem::module(STR("engine.dll")))
            .as<void*>();
    DATA.update_clientside_animation = mem::sig(STR("56 8B F1 80 BE ? ? ? ? ? 74 27"))
            .scan(mem::module(STR("client.dll")))
            .as<void*>();

    DATA.create_move = hack::virtual_func<void*>(sdk::client_mode, 21);
    DATA.set_cursor_always_visible = hack::virtual_func<void*>(sdk::surface, 52);
    DATA.lock_cursor = hack::virtual_func<void*>(sdk::surface, 62);
    DATA.run_frame = hack::virtual_func<void*>(sdk::surface, 5);
    DATA.frame_stage_notify = hack::virtual_func<void*>(sdk::client, 35);
    DATA.draw_model_execute = hack::virtual_func<void*>(sdk::model_render, 19);
    DATA.override_view = hack::virtual_func<void*>(sdk::client_mode, 16);

    if (FAILED(MH_CreateHook(DATA.create_move, (void*)&hooks::create_move, (void**)&hooks::o_create_move)))
        return false;
    if (FAILED(MH_CreateHook(DATA.set_cursor_always_visible, (void*)&hooks::set_cursor_always_visible, (void**)&hooks::o_set_cursor_always_visible)))
        return false;
    if (FAILED(MH_CreateHook(DATA.lock_cursor, (void*)&hooks::lock_cursor, (void**)&hooks::o_lock_cursor)))
        return false;
    if (FAILED(MH_CreateHook(DATA.send_move, (void*)&hooks::send_move, (void**)&hooks::o_send_move)))
        return false;
    if (FAILED(MH_CreateHook(DATA.run_frame, (void*)&hooks::run_frame, (void**)&hooks::o_run_frame)))
        return false;
    if (FAILED(MH_CreateHook(DATA.frame_stage_notify, (void*)&hooks::frame_stage_notify, (void**)&hooks::o_frame_stage_notify)))
        return false;
    if (FAILED(MH_CreateHook(DATA.fire_bullets, (void*) &hooks::fire_bullets, (void**)&hooks::o_fire_bullets)))
        return false;
    if (FAILED(MH_CreateHook(DATA.draw_model_execute, (void*)&hooks::draw_model_execute, (void**)&hooks::o_draw_model_execute)))
        return false;
    if (FAILED(MH_CreateHook(DATA.update, (void*)&hooks::update, (void**)&hooks::o_update)))
        return false;
    if (FAILED(MH_CreateHook(DATA.override_view, (void*)&hooks::override_view, (void**)&hooks::o_override_view)))
        return false;
    if (FAILED(MH_CreateHook(DATA.encode_float, (void*)&hooks::encode_float, (void**)&hooks::o_encode_float)))
        return false;
    if (FAILED(MH_CreateHook(DATA.update_clientside_animation, (void*) &hooks::update_clientside_animation, (void**)&hooks::o_update_clientside_animation)))
        return false;

    if (FAILED(MH_EnableHook(DATA.create_move)))
        return false;
    if (FAILED(MH_EnableHook(DATA.set_cursor_always_visible)))
        return false;
    if (FAILED(MH_EnableHook(DATA.lock_cursor)))
        return false;
    if (FAILED(MH_EnableHook(DATA.send_move)))
        return false;
    if (FAILED(MH_EnableHook(DATA.run_frame)))
        return false;
    if (FAILED(MH_EnableHook(DATA.frame_stage_notify)))
        return false;
    if (FAILED(MH_EnableHook(DATA.fire_bullets)))
        return false;
    if (FAILED(MH_EnableHook(DATA.draw_model_execute)))
        return false;
    if (FAILED(MH_EnableHook(DATA.update)))
        return false;
    if (FAILED(MH_EnableHook(DATA.override_view)))
        return false;
    if (FAILED(MH_EnableHook(DATA.encode_float)))
        return false;
    if (FAILED(MH_EnableHook(DATA.update_clientside_animation)))
        return false;

    return true;
}

void core::unhook_game() {
    MH_DisableHook(DATA.create_move);
    MH_DisableHook(DATA.set_cursor_always_visible);
    MH_DisableHook(DATA.lock_cursor);
    MH_DisableHook(DATA.send_move);
    MH_DisableHook(DATA.run_frame);
    MH_DisableHook(DATA.frame_stage_notify);
    MH_DisableHook(DATA.fire_bullets);
    MH_DisableHook(DATA.draw_model_execute);
    MH_DisableHook(DATA.update);
    MH_DisableHook(DATA.override_view);
    MH_DisableHook(DATA.encode_float);
    MH_DisableHook(DATA.update_clientside_animation);

    MH_RemoveHook(DATA.create_move);
    MH_RemoveHook(DATA.set_cursor_always_visible);
    MH_RemoveHook(DATA.lock_cursor);
    MH_RemoveHook(DATA.send_move);
    MH_RemoveHook(DATA.run_frame);
    MH_RemoveHook(DATA.frame_stage_notify);
    MH_RemoveHook(DATA.fire_bullets);
    MH_RemoveHook(DATA.draw_model_execute);
    MH_RemoveHook(DATA.update);
    MH_RemoveHook(DATA.override_view);
    MH_RemoveHook(DATA.encode_float);
    MH_RemoveHook(DATA.update_clientside_animation);
}

bool core::load_netvars() {
    auto c = sdk::client->get_all_classes();
    if (!c)
        return false;

    while (c) {
        auto t = c->table;
        if (t) {
            sdk::netvars[rage::joaat(t->name)] = sdk::datatable {
                t->name,
                dump_recv_table(t)
            };
        }

        c = c->next;
    }

    return true;
}

bool core::patch_game() {
    //mem::region(mem::sig(STR("E8 ? ? ? ? 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 3D ? ? ? ? ?"))
    //                    .scan(mem::module(STR("client.dll")))
    //                    .sub(5), 10)
    //        .protect_start(PAGE_EXECUTE_READWRITE)
    //        .patch(STR("\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"), DATA.patch_cam_think)
    //        .protect_end();

    return true;
}

void core::unpatch_game() {
    //mem::region(mem::sig(STR("E8 ? ? ? ? 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 3D ? ? ? ? ?"))
    //                    .scan(mem::module(STR("client.dll")))
    //                    .sub(5), 10)
    //        .protect_start(PAGE_EXECUTE_READWRITE)
    //        .patch(DATA.patch_cam_think)
    //        .protect_end();
}
