//
// Created by panzerfaust on 5/25/2020.
//

#include <ev0lve.h>
#include <core/hooks.h>
#include <core/menu.h>
#include <core/cfg.h>
#include <core/data/small.h>
#include <renderer/renderer.h>
#include <input/input.h>
#include <misc/xor.h>
#include <misc/utils.h>

#include <hacks/esp.h>
#include <hacks/debug_tools.h>

inline struct {
    IDirect3DStateBlock9* pixel_state = nullptr;
    IDirect3DVertexDeclaration9* vertex_desc = nullptr;
    IDirect3DVertexShader9* vertex_shader = nullptr;
    DWORD color_write = 0, srgb_write = 0;

    inline void backup(IDirect3DDevice9* device) {
        device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
        device->GetVertexDeclaration(&vertex_desc);
        device->GetVertexShader(&vertex_shader);

        device->GetRenderState(D3DRS_COLORWRITEENABLE, &color_write);
        device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgb_write);
        device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
        device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
        device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        device->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
        device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, false);
    }

    inline void restore(IDirect3DDevice9* device) {
        device->SetRenderState(D3DRS_COLORWRITEENABLE, color_write);
        device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgb_write);

        pixel_state->Apply();
        pixel_state->Release();
        device->SetVertexDeclaration(vertex_desc);
        device->SetVertexShader(vertex_shader);
    }
} DX_BACKUP;

struct {
} GRAPHS;

long __stdcall hooks::present(IDirect3DDevice9* device, RECT* a, RECT* b, HWND c, RGNDATA* d) {
    if (!hack::is_running)
        return o_present(device, a, b, c, d);

    if (!renderer::adapter) {
        auto wnd = FindWindowA(STR("Valve001"), nullptr);
        if (!wnd)
            return o_present(device, a, b, c, d);

        renderer::adapter = new renderer::adapter_dx9(wnd, device);
        input::ctx->init(wnd);

        renderer::adapter->fonts[JOAAT("esp_small")] = renderer::font(DATA_ESP_SMALL_FONT, 25600, 10.f);
        renderer::adapter->fonts[JOAAT("esp_small_outline")] = renderer::font(
                DATA_ESP_SMALL_FONT,
                25600,
                10.f,
                0x00,
                0xFF,
                true
        );
        renderer::adapter->fonts[JOAAT("esp_main")] = renderer::font(
                STR("c:/windows/fonts/verdana.ttf"),
                12.f,
                0x0000,
                0xFFFF
        );
        renderer::adapter->fonts[JOAAT("esp_bold")] = renderer::font(
                STR("c:/windows/fonts/verdanab.ttf"),
                12.f,
                0x0000,
                0xFFFF
        );

        menu::init();
    }

    input::ctx->handle_pre_frame();
    if (input::ctx->keys_clicked[cfg::storage[JOAAT("menu_key")].get<uint32_t>(VK_INSERT)])
        menu::set_opened(!menu::is_opened());

    DX_BACKUP.backup(device);
    renderer::adapter->prepare();
    {
        hack::esp.draw();
        hack::debug_tools.draw();

        menu::draw();
    }
    renderer::adapter->render();
    renderer::adapter->end();
    DX_BACKUP.restore(device);

    input::ctx->handle_frame();
    return o_present(device, a, b, c, d);
}