//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_EV0LVE_H
#define EV0LVE_TF_EV0LVE_H

#define EVOLVE_GAME "Team Fortress 2"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <misc/joaat.h>

namespace hack {
    const uint32_t APP_SYSTEM_M = 5;

    inline HMODULE module = nullptr;
    inline bool is_running = true;

    template<typename FN>
    inline FN virtual_func(void* b, uint32_t i) {
        return reinterpret_cast<FN>((*(uint32_t**)b)[i]);
    }

    template<typename T>
    inline T& get(void* F, uint32_t o) {
        return *reinterpret_cast<T*>(reinterpret_cast<uint32_t>(F) + o);
    }

    int exception_handler(int code, const char* fn);
}

#if _DEBUG
    #pragma comment(lib, "minhook_d.lib")
#else
    #pragma comment(lib, "minhook.lib")
#endif

#endif //EV0LVE_TF_EV0LVE_H
