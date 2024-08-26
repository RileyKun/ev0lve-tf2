//
// Created by panzerfaust on 5/25/2020.
//

#include <misc/xor.h>
#include <misc/utils.h>
#include <misc/logger.h>
#include <misc/mem.h>

#include <renderer/renderer.h>

void utils::show_message(const std::string &title, const std::string &msg) {
    MessageBoxA(nullptr, msg.c_str(), title.c_str(), MB_SYSTEMMODAL | MB_OK);
}

void utils::unload(HMODULE mod) {
    static auto fn = GetProcAddress(GetModuleHandleA(STR("kernel32.dll")), STR("FreeLibraryAndExitThread"));
    ((void(__stdcall*)(HMODULE, uint32_t))fn)(mod, 0);
}

void utils::wait_for_module(const std::string &mod, uint32_t max_time) {
    auto notified = false;

    auto waited = 0U;
    while (!GetModuleHandleA(mod.c_str()) && waited < max_time) {
        if (!notified) {
            logger::log(STR("Waiting for %s..."), mod.c_str());
            notified = true;
        }

        Sleep(250);
        waited += 250;
    }
}

std::string utils::format(const char* txt, ...) {
    char buffer[8912]{0};

    va_list va;
    va_start(va, txt);
    vsprintf_s(buffer, 8912, txt, va);
    va_end(va);

    return buffer;
}