//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_HOOKS_H
#define EV0LVE_TF_HOOKS_H

#include <sdk/sdk.h>
#include <renderer/adapter_dx9.h>

namespace hooks {
    using fnPresent = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
    using fnReset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    using fnCreateMove = bool(__thiscall*)(void*, float, sdk::user_cmd*);
    using fnSetCursorAlwaysVisible = void(__thiscall*)(void*, bool);
    using fnLockCursor = void(__thiscall*)(void*);
    using fnCL_SendMove = void(*)();
    using fnRunFrame = void(__thiscall*)(void*);
    using fnFrameStageNotify = void(__thiscall*)(void*, int);
    using fnFireBullets = void(__thiscall*)(sdk::entity*, sdk::entity*, const sdk::fire_bullets_info&, bool, int, int);
    using fnDoAnimationEvent = void(__thiscall*)(sdk::entity*, int, int);
    using fnDrawModelExecute = void(__thiscall*)(void*, const sdk::draw_model_state&, const sdk::model_render_info&, sdk::matrix3x4*);
    using fnUpdate = void(__thiscall*)(void*);
    using fnOverrideView = void(__thiscall*)(void*, sdk::view_setup*);
    using fnEncodeFloat = void(__cdecl*)(sdk::send_prop*, float, sdk::bf_write*, int);
    using fnUpdateClientsideAnimation = void(__thiscall*)(void*);

    inline fnPresent o_present = nullptr;
    inline fnReset o_reset = nullptr;
    inline WNDPROC o_wndproc = nullptr;
    inline fnCreateMove o_create_move = nullptr;
    inline fnSetCursorAlwaysVisible o_set_cursor_always_visible = nullptr;
    inline fnLockCursor o_lock_cursor = nullptr;
    inline fnCL_SendMove o_send_move = nullptr;
    inline fnRunFrame o_run_frame = nullptr;
    inline fnFrameStageNotify o_frame_stage_notify = nullptr;
    inline fnFireBullets o_fire_bullets = nullptr;
    inline fnDoAnimationEvent o_do_animation_events = nullptr;
    inline fnDrawModelExecute o_draw_model_execute = nullptr;
    inline fnUpdate o_update = nullptr;
    inline fnOverrideView o_override_view = nullptr;
    inline fnEncodeFloat o_encode_float = nullptr;
    inline fnUpdateClientsideAnimation o_update_clientside_animation = nullptr;

    long __stdcall present(IDirect3DDevice9* device, RECT* a, RECT* b, HWND c, RGNDATA* d);
    long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* a);
    long __stdcall wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
    bool __fastcall create_move(void* ecx, void*, float time, sdk::user_cmd* cmd);
    void __fastcall set_cursor_always_visible(void* ecx, void*, bool visible);
    void __fastcall lock_cursor(void* ecx, void*);
    void __stdcall send_move();
    void __fastcall run_frame(void* ecx, void*);
    void __fastcall frame_stage_notify(void* ecx, void*, int stage);
    void __fastcall do_animation_events(sdk::entity* ecx, void*, int event, int data);
    void __fastcall draw_model_execute(void* ecx, void*, const sdk::draw_model_state& state, const sdk::model_render_info& info, sdk::matrix3x4* mtx);
    void __fastcall update(void* ecx, void*);
    void __fastcall override_view(void* ecx, void*, sdk::view_setup* setup);
    void __cdecl encode_float(sdk::send_prop* prop, float val, sdk::bf_write* out, int object_id);
    void __fastcall update_clientside_animation(sdk::entity *ecx, void *);
    void __fastcall fire_bullets(sdk::entity* ecx, void*, sdk::entity* wep, const sdk::fire_bullets_info& info,
            bool do_eff, int dmg_type, int custom_dmg);
}

#endif //EV0LVE_TF_HOOKS_H
