//
// Created by panzerfaust on 5/25/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <renderer/renderer.h>

long __stdcall hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* a) {
    if (!hack::is_running)
        return o_reset(device, a);

    renderer::adapter->destroy_objects();
    return o_reset(device, a);
}