//
// Created by panzerfaust on 5/25/2020.
//

#include <core/cfg.h>
#include <core/menu.h>
#include <misc/xor.h>
#include <misc/logger.h>
#include <sdk/sdk.h>
#include <core/game.h>

namespace tab_misc {
    inline void group_configs(gui::tab* tab) {
        static std::string _cfg_input;
        auto refresh = new gui::button(JOAAT("misc_refresh"), STR("Refresh"), _VEC2(164.f, 20.f));
        refresh->callback = []() {
            auto cfg_list = gui::ctx->find_element<gui::listbox>(JOAAT("misc_cfg_list"));
            for (auto& e : cfg_list->elements->elements)
                delete e;
            cfg_list->elements->elements.clear();

            for (auto& c : cfg::ctx->refresh())
                cfg_list->elements->elements.emplace_back(new gui::listbox_text_entry(rage::joaat(c.c_str()), c));
        };

        auto create = new gui::button(JOAAT("misc_create"), STR("Create"), _VEC2(164.f, 20.f));
        create->callback = [&]() {
            cfg::ctx->save(_cfg_input + STR(".dat"));
            gui::ctx->find_element<gui::button>(JOAAT("misc_refresh"))->callback();
        };

        auto remove = new gui::button(JOAAT("misc_remove"), STR("Remove"), _VEC2(164.f, 20.f));
        remove->callback = []() {
            auto cfg_list = gui::ctx->find_element<gui::listbox>(JOAAT("misc_cfg_list"));

            for (auto& c : cfg::ctx->files) {
                if (rage::joaat(c.c_str()) == cfg_list->selected_id)
                    std::filesystem::remove(cfg::ctx->path + STR("/") + c);
            }

            gui::ctx->find_element<gui::button>(JOAAT("misc_refresh"))->callback();
        };

        auto load = new gui::button(JOAAT("misc_load"), STR("Load"), _VEC2(164.f, 20.f));
        load->callback = []() {
            auto cfg_list = gui::ctx->find_element<gui::listbox>(JOAAT("misc_cfg_list"));

            for (auto& c : cfg::ctx->files) {
                if (rage::joaat(c.c_str()) == cfg_list->selected_id)
                    cfg::ctx->load(c);
            }

            menu::reset_colors();
        };

        auto save = new gui::button(JOAAT("misc_save"), STR("Save"), _VEC2(164.f, 20.f));
        save->callback = []() {
            auto cfg_list = gui::ctx->find_element<gui::listbox>(JOAAT("misc_cfg_list"));

            for (auto& c : cfg::ctx->files) {
                if (rage::joaat(c.c_str()) == cfg_list->selected_id)
                    cfg::ctx->save(c);
            }
        };

        auto reset = new gui::button(JOAAT("misc_reset"), STR("Reset"), _VEC2(164.f, 20.f));
        reset->callback = []() {
            cfg::config_system::reset();
            menu::reset_colors();
        };

        auto group = new gui::stack_layout(JOAAT("misc_configs"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
        group->elements.emplace_back(new gui::listbox(JOAAT("misc_cfg_list"), _VEC2(164.f, 160.f)));
        group->elements.emplace_back(refresh);
        group->elements.emplace_back(load);
        group->elements.emplace_back(save);
        group->elements.emplace_back(reset);
        group->elements.emplace_back(remove);
        group->elements.emplace_back(new gui::text_input(JOAAT("misc_cfg_name"), &_cfg_input));
        group->elements.emplace_back(create);

        auto _ = menu::make_group(
                JOAAT("misc_configs"),
                STR("PRESETS"),
                _VEC2(570.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }

    inline void group_misc(gui::tab* tab) {
        auto group = new gui::stack_layout(JOAAT("misc_misc"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
        group->elements.emplace_back(new gui::text_label(JOAAT("misc_fov_text"), STR("Override FOV")));
        group->elements
             .emplace_back(
                     new gui::slider(
                             JOAAT("misc_fov"),
                             0.f,
                             60.f,
                             _CFG("misc_fov").raw<float>(0.f),
                             1.f,
                             STR("%.0f°")
                     )
             );
        group->elements.emplace_back(
                new gui::checkbox(
                        JOAAT("misc_autojump"), STR("Auto jump"), _CFG("misc_autojump").raw<bool>(false)
                )
        );
        group->elements.emplace_back(
                new gui::checkbox(
                        JOAAT("misc_auto_strafe"), STR("Auto strafe"), _CFG("misc_auto_strafe").raw<bool>(false)
                )
        );
        // group->elements.emplace_back(new gui::slider(JOAAT("misc_shift"), 0.f, 13.f, _CFG("misc_shift").raw<float>(0.f)));

        auto _ = menu::make_group(
                JOAAT("misc_misc"),
                STR("MISCELLANEOUS"),
                _VEC2(20.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }

    inline void group_settings(gui::tab* tab) {
        auto group = new gui::stack_layout(JOAAT("misc_settings"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
        group->elements.emplace_back(
                menu::make_line(
                        JOAAT("misc_menu_key_layout"), {
                                new gui::text_label(JOAAT("misc_menu_key_text"), STR("Menu key")),
                                new gui::hotkey(JOAAT("misc_menu_key"), _CFG("menu_key").raw<uint32_t>(VK_INSERT))
                        }
                )
        );
        group->elements.emplace_back(
                menu::make_line(
                        JOAAT("misc_menu_col_layout"), {
                                new gui::text_label(JOAAT("misc_menu_col_text"), STR("Menu color")),
                                new gui::color_picker(
                                        JOAAT("misc_menu_col"),
                                        _CFG("menu_color").raw<_COLOR>(gui::ctx->colors.accent),
                                        false
                                )
                        }
                )
        );

        auto unload = new gui::button(JOAAT("misc_unload"), STR("Unload"), _VEC2(164.f, 20.f));
        unload->callback = []() {
            hack::is_running = false;
        };

        auto print_conds = new gui::button(JOAAT("misc_print_conds"), STR("Debug"), _VEC2(164.f, 20.f));
        print_conds->callback = []() {

        };

        group->elements.emplace_back(unload);
        group->elements.emplace_back(print_conds);

        auto _ = menu::make_group(
                JOAAT("misc_settings"),
                STR("SETTINGS"),
                _VEC2(295.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }
}

gui::title_tab* menu::make_tab_misc() {
    auto tab = new gui::title_tab(JOAAT("misc"), STR("SETTINGS"), STR("A"));
    tab_misc::group_misc(tab);
    tab_misc::group_settings(tab);
    tab_misc::group_configs(tab);

    return tab;
}