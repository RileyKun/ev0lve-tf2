cmake_minimum_required(VERSION 3.16)
project(ev0lve_tf)

include_directories(
        "."
        "./deps/minhook/include"

        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
        "D:/Sources/freetype-2.10.2/include"
)

link_directories (
        "./deps/minhook/lib"

        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
        "D:/Sources/freetype-2.10.2/objs/Win32/Release Static"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ev0lve_tf SHARED
        main.cpp
        ev0lve.h
        gui/controls/button.cpp
        gui/controls/checkbox.cpp
        gui/controls/colorpicker.cpp
        gui/controls/combobox.cpp
        gui/controls/content_tab.cpp
        gui/controls/group.cpp
        gui/controls/hotkey.cpp
        gui/controls/label.cpp
        gui/controls/large_window.cpp
        gui/controls/layout.cpp
        gui/controls/listbox.cpp
        gui/controls/listbox_entry.cpp
        gui/controls/listbox_game_entry.cpp
        gui/controls/listbox_product_entry.cpp
        gui/controls/listbox_text_entry.cpp
        gui/controls/progressbar.cpp
        gui/controls/scroll_layout.cpp
        gui/controls/slider.cpp
        gui/controls/small_window.cpp
        gui/controls/stack_layout.cpp
        gui/controls/text_input.cpp
        gui/controls/title_tab.cpp
        gui/data/game.h
        gui/data/product.h
        gui/fonts.h
        gui/gui.h
        gui/gui.cpp
        gui/textures.h
        input/input.h
        input/input.cpp
        renderer/adapter_dx9.cpp
        renderer/adapter_dx9.h
        renderer/data_texture.h
        renderer/renderer.cpp
        renderer/renderer.h
        renderer/tools_dx.h
        renderer/tools_math.h
        types/types.h
        misc/joaat.h
        misc/utils.h
        misc/utils.cpp
        misc/logger.cpp
        misc/logger.h
        misc/xor.h
        misc/rand.h
        misc/mem.h
        core/core.h
        core/core.cpp
        core/hooks.h
        core/hooks/present.cpp
        core/hooks/reset.cpp
        core/menu.h
        core/menu.cpp
        core/hooks/wndproc.cpp
        core/cfg.cpp
        core/cfg.h
        core/tabs/tab_misc.cpp
        sdk/sdk.h
        sdk/global_vars.h
        sdk/client.h
        core/hooks/create_move.cpp
        sdk/surface.h
        core/hooks/set_cursor_always_visible.cpp
        core/hooks/lock_cursor.cpp
        core/data/cursor.h
        sdk/engine.h
        sdk/datamap.h
        sdk/vector.h
        sdk/entity_list.h
        sdk/entity.h
        sdk/handle.h
        sdk/client_class.h
        sdk/collideable.h
        sdk/netvars.h
        sdk/user_cmd.h
        sdk/game_input.h
        hacks/autojump.h
        hacks/autojump.cpp
        misc/crc32.h
        misc/crc32.cpp
        sdk/client_state.h
        sdk/net_channel.h
        sdk/utl.h
        core/hooks/send_move.cpp
        sdk/client_mode.h
        sdk/sdk.cpp
        sdk/maths.h
        sdk/color.h
        sdk/cvar.h
        hacks/esp.h
        hacks/esp.cpp
        core/tabs/tab_visuals.cpp
        core/hooks/run_frame.cpp
        sdk/debug_overlay.h
        core/data/small.h
        sdk/entity.cpp
        core/hooks/frame_stage_notify.cpp
        hacks/debug_tools.h
        hacks/debug_tools.cpp
        sdk/localize.h
        sdk/game_event.h
        hacks/game_events.h
        hacks/game_events.cpp
        core/hooks/fire_bullets.cpp
        core/hooks/do_animation_event.cpp
        sdk/materialsystem.h
        sdk/keyvalues.h
        hacks/chams.h
        hacks/chams.cpp
        sdk/modelrender.h
        sdk/renderview.h
        sdk/studio.h
        sdk/matrix.h
        core/hooks/draw_model_execute.cpp
        sdk/studio_render_context.h
        sdk/glow_manager.h
        core/hooks/update.cpp
        sdk/view_setup.h
        core/hooks/override_view.cpp
        sdk/engine_trace.h
        core/tabs/tab_aimbot.cpp
        core/tabs/tab_lua.cpp
        sdk/model.h
        sdk/modelinfo.h
        hacks/aimbot.cpp
        hacks/aimbot.h
        hacks/lc.h
        hacks/lc.cpp
        sdk/netmessage.h
        sdk/bitbuf.h
        sdk/dwords.h
        hacks/prediction.h
        hacks/prediction.cpp
        sdk/game_movement.h
        sdk/game_prediction.h sdk/thirdperson_manager.h hacks/antiaim.h hacks/antiaim.cpp core/hooks/encode_float.cpp core/hooks/animstate_update.cpp core/game.h core/game.cpp)

target_compile_options(ev0lve_tf PRIVATE
        $<$<CONFIG:Release>:
        /O3
        /Zc:threadSafeInit-
        /GS-
        >

        /utf-8
        )