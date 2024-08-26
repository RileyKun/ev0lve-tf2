//
// Created by panzerfaust on 5/28/2020.
//

#include <core/cfg.h>
#include <core/menu.h>
#include <misc/xor.h>
#include <sdk/sdk.h>

namespace sub_tab_esp {
    inline gui::tab* players() {
        auto tab = new gui::content_tab(JOAAT("vis_esp_players"), STR("PLAYERS"));
        tab->is_selected = true;
        {
            auto group = new gui::stack_layout(JOAAT("vis_esp_players_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(
                    new gui::checkbox(
                            JOAAT("vis_esp_enable"), STR("Enable"), _CFG("vis_esp_enable").raw<bool>(false)
                    )
            );
            group->elements.emplace_back(
                    new gui::checkbox(
                            JOAAT("vis_esp_dormant"), STR("Show dormant"), _CFG("vis_esp_dormant").raw<bool>(false)
                    )
            );
            group->elements.emplace_back(
                    new gui::checkbox(
                            JOAAT("vis_esp_team"), STR("Show teammates"), _CFG("vis_esp_team").raw<bool>(false)
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_esp_box_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_esp_box"),
                                            STR("Bounding box"),
                                            _CFG("vis_esp_box").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_esp_box_col"),
                                            _CFG("vis_esp_box_col").raw<_COLOR>(_COLOR(255, 255, 255))
                                    )
                            }
                    )
            );
            group->elements.emplace_back(
                    new gui::checkbox(
                            JOAAT("vis_esp_hp"), STR("Health bar"), _CFG("vis_esp_hp").raw<bool>(false)
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_esp_name_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_esp_name"),
                                            STR("Name"),
                                            _CFG("vis_esp_name").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_esp_name_col"),
                                            _CFG("vis_esp_name_col").raw<_COLOR>(_COLOR(255, 255, 255))
                                    )
                            }
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_esp_wep_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_esp_wep"),
                                            STR("Weapon"),
                                            _CFG("vis_esp_wep").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_esp_wep_col"),
                                            _CFG("vis_esp_wep_col").raw<_COLOR>(_COLOR(255, 255, 255))
                                    )
                            }
                    )
            );

            // Will add later...
            // group->elements.emplace_back(menu::make_line(JOAAT("vis_esp_ammo_layout"), {
            //         new gui::checkbox(JOAAT("vis_esp_ammo"), STR("Ammo"), _CFG("vis_esp_ammo").raw<bool>(false)),
            //         new gui::color_picker(JOAAT("vis_esp_ammo_col"), _CFG("vis_esp_ammo_col").raw<_COLOR>(_COLOR(255, 255, 255)))
            // }));

            group->elements.emplace_back(new gui::text_label(JOAAT("vis_esp_flags_label"), STR("Flags")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("vis_esp_flags"), {
                                    STR("Class"), STR("Distance"), STR("Cloak"), STR("Aiming"), STR("ÜberCharge")
                            }, _CFG("vis_esp_flags").raw<_BITS>(0), true
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_esp_offscreen_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_esp_offscreen"),
                                            STR("Offscreen ESP"),
                                            _CFG("vis_esp_offscreen").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_esp_offscreen_col"),
                                            _CFG("vis_esp_offscreen_col").raw<_COLOR>(_COLOR(255, 255, 255))
                                    )
                            }
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("vis_esp_players_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }

        return tab;
    }

    inline gui::tab* objects() {
        auto tab = new gui::content_tab(JOAAT("vis_esp_objs"), STR("OBJECTS"));
        return tab;
    }

    inline gui::tab* guns() {
        auto tab = new gui::content_tab(JOAAT("vis_esp_guns"), STR("GUNS"));
        return tab;
    }

    inline gui::tab* projectiles() {
        auto tab = new gui::content_tab(JOAAT("vis_esp_proj"), STR("PROJECTILES"));
        return tab;
    }
}
namespace sub_tab_chams {
    inline gui::tab* players() {
        auto tab = new gui::content_tab(JOAAT("vis_chams_players"), STR("PLAYERS"));
        tab->is_selected = true;
        {
            auto group = new gui::stack_layout(JOAAT("vis_chams_players_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("vis_chams_players_type"), {
                                    STR("Default"), STR("Flat")
                            }, _CFG("vis_chams_players_type").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_chams_enemies_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_chams_enemies"),
                                            STR("Enemies (visible)"),
                                            _CFG("vis_chams_enemies").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_chams_enemies_col"),
                                            _CFG("vis_chams_enemies_col").raw<_COLOR>(_COLOR_WHITE)
                                    )
                            }
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_chams_enemies_i_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_chams_enemies_i"),
                                            STR("Enemies (invisible)"),
                                            _CFG("vis_chams_enemies_i").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_chams_enemies_i_col"),
                                            _CFG("vis_chams_enemies_i_col").raw<_COLOR>(_COLOR_WHITE)
                                    )
                            }
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_chams_allies_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_chams_allies"),
                                            STR("Teammates"),
                                            _CFG("vis_chams_allies").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_chams_allies_col"),
                                            _CFG("vis_chams_allies_col").raw<_COLOR>(_COLOR_WHITE)
                                    )
                            }
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("vis_chams_players_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }

        return tab;
    }

    inline gui::tab* self() {
        auto tab = new gui::content_tab(JOAAT("vis_chams_self"), STR("SELF"));
        {
            auto group = new gui::stack_layout(JOAAT("vis_chams_self_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("vis_chams_self_type"), {
                                    STR("Default"), STR("Flat")
                            }, _CFG("vis_chams_self_type").raw<_BITS>(0)
                    )
            );

            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_chams_self_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_chams_self"),
                                            STR("Enable"),
                                            _CFG("vis_chams_self").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_chams_self_col"),
                                            _CFG("vis_chams_self_col").raw<_COLOR>(_COLOR_WHITE)
                                    )
                            }
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("vis_chams_self_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }
        return tab;
    }

    inline gui::tab* objects() {
        auto tab = new gui::content_tab(JOAAT("vis_chams_objects"), STR("OBJECTS"));
        {
            auto group = new gui::stack_layout(JOAAT("vis_chams_obj_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("vis_chams_obj_type"), {
                                    STR("Default"), STR("Flat")
                            }, _CFG("vis_chams_obj_type").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(
                    menu::make_line(
                            JOAAT("vis_chams_obj_layout"), {
                                    new gui::checkbox(
                                            JOAAT("vis_chams_obj"),
                                            STR("Enable"),
                                            _CFG("vis_chams_obj").raw<bool>(false)
                                    ),
                                    new gui::color_picker(
                                            JOAAT("vis_chams_obj_col"),
                                            _CFG("vis_chams_obj_col").raw<_COLOR>(_COLOR_WHITE)
                                    )
                            }
                    )
            );
            group->elements.emplace_back(
                    new gui::checkbox(
                            JOAAT("vis_chams_obj_team"),
                            STR("Team objects"),
                            _CFG("vis_chams_obj_team").raw<bool>(false)
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("vis_chams_obj_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }
        return tab;
    }
}
namespace tab_visuals {
    inline void group_player_esp(gui::tab* tab) {
        auto group = new gui::layout(JOAAT("vis_esp"), _VEC2(0.f, 0.f), _VEC2(242.f, 360.f));
        group->elements.emplace_back(sub_tab_esp::players());
        group->elements.emplace_back(sub_tab_esp::objects());
        group->elements.emplace_back(sub_tab_esp::guns());
        group->elements.emplace_back(sub_tab_esp::projectiles());

        auto _ = menu::make_group(JOAAT("vis_esp"), STR("ESP"), _VEC2(20.f, 70.f), _VEC2(260.f, 400.f), group);
        tab->elements.emplace_back(_);
    }

    inline void group_chams(gui::tab* tab) {
        auto group = new gui::layout(JOAAT("vis_chams"), _VEC2(0.f, 0.f), _VEC2(242.f, 360.f));
        group->elements.emplace_back(sub_tab_chams::players());
        group->elements.emplace_back(sub_tab_chams::self());
        group->elements.emplace_back(sub_tab_chams::objects());

        auto _ = menu::make_group(
                JOAAT("vis_chams"),
                STR("COLORED MODELS"),
                _VEC2(295.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }

    inline void group_other(gui::tab* tab) {
        auto group = new gui::stack_layout(JOAAT("vis_other"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
        group->elements.emplace_back(
                menu::make_line(
                        JOAAT("vis_other_tp_layout"), {
                                new gui::text_label(JOAAT("vis_other_tp"), STR("Thirdperson")),
                                new gui::hotkey(
                                        JOAAT("vis_other_tp_k"),
                                        _CFG("vis_other_tp_k").raw<uint32_t>(0),
                                        _CFG("vis_other_tp_b").raw<uint32_t>(2)
                                )
                        }
                )
        );
        group->elements
             .emplace_back(
                     new gui::slider(
                             JOAAT("vis_other_tp_dist"),
                             60.f,
                             180.f,
                             _CFG("vis_other_tp_dist").raw<float>(150.f),
                             1.f,
                             STR("%.0fu")
                     )
             );

        auto _ = menu::make_group(JOAAT("vis_other"), STR("OTHER"), _VEC2(570.f, 70.f), _VEC2(260.f, 400.f), group);
        tab->elements.emplace_back(_);
    }
}

gui::title_tab* menu::make_tab_vis() {
    auto tab = new gui::title_tab(JOAAT("vis"), STR("VISUALS"), STR("C"));
    tab_visuals::group_player_esp(tab);
    tab_visuals::group_chams(tab);
    tab_visuals::group_other(tab);

    return tab;
}