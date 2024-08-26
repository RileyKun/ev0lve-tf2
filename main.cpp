//
// Created by panzerfaust on 5/25/2020.
//
#include <ev0lve.h>
#include <core/core.h>
#include <core/cfg.h>
#include <misc/utils.h>
#include <misc/logger.h>
#include <misc/xor.h>
#include <renderer/renderer.h>
#include <gui/gui.h>
#include <hacks/game_events.h>

int hack::exception_handler(int code, const char* fn) {
    utils::show_message(STR("Fatal error"), utils::format(STR("Whoops.\n\n0x%p is happened in %s\nPlease tell to developer!"), code, fn));
    return EXCEPTION_CONTINUE_EXECUTION;
}

uint32_t __stdcall main_thread(void*) {
    logger::create_console();

    if (!std::filesystem::exists(STR("ev0lve")))
        std::filesystem::create_directory(STR("ev0lve"));

    cfg::ctx->magic = (uint16_t )0xEF00;
    cfg::ctx->path = STR("ev0lve");

    utils::wait_for_module(STR("engine.dll"));
    utils::wait_for_module(STR("client.dll"));
    utils::wait_for_module(STR("vguimatsurface.dll"));
    utils::wait_for_module(STR("gameoverlayrenderer.dll"));
    utils::wait_for_module(STR("vaudio_miles.dll"));

    if (!core::init()) {
        utils::show_message(STR("Load failed"), STR("Couldn't load hack. Please, contact developer!"));

        core::shutdown();
        cfg::config_system::destroy();
        renderer::adapter->destroy_texture(gui::ctx->texture);
        renderer::adapter->destroy_objects();

        logger::free_console();

        Sleep(NUM(250));
        utils::unload(hack::module);
    }

    while (hack::is_running)
        Sleep(NUM(250));

    core::shutdown();
    cfg::config_system::destroy();
    renderer::adapter->destroy_texture(gui::ctx->texture);
    renderer::adapter->destroy_objects();

    logger::free_console();
    Beep(NUM(250), NUM(250));

    Sleep(NUM(250));
    utils::unload(hack::module);
    return 0;
}

int __stdcall DllMain(HINSTANCE module, uint32_t reason, void*) {
    if (reason == DLL_PROCESS_ATTACH) {
        hack::module = module;
        CreateThread(
                nullptr, 0,
                reinterpret_cast<PTHREAD_START_ROUTINE>(main_thread), nullptr,
                0, nullptr);
    }

    return 1;
}