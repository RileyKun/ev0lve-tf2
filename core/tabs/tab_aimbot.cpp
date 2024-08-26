//
// Created by panzerfaust on 6/5/2020.
//

#include <core/cfg.h>
#include <core/menu.h>
#include <misc/xor.h>

namespace subtab_antiaim {
    inline gui::tab* standing() {
        auto tab = new gui::content_tab(JOAAT("aim_antiaim_standing"), STR("STANDING"));
        tab->is_selected = true;
        {
            auto group = new gui::stack_layout(JOAAT("aim_antiaim_standing_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_as_pitch_t"), STR("Pitch")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_as_pitch"), {
                                    STR("Disabled"), STR("Up"), STR("Down"), STR("Fake up"), STR("Fake down")
                            }, _CFG("aim_as_pitch").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_as_yaw_t"), STR("Yaw")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_as_yaw"), {
                                    STR("Disabled"), STR("Forward"), STR("Backward"), STR("Jitter"), STR("Rotate")
                            }, _CFG("aim_as_yaw").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_as_yaw_add"),
                            -180.f,
                            180.f,
                            _CFG("aim_as_yaw_add").raw<float>(),
                            1.f,
                            STR("%.0f°")
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_as_yaw_mod_t"), STR("Modifier")));
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_as_yaw_mod"), 0.f, 360.f, _CFG("aim_as_yaw_mod").raw<float>(), 1.f, STR("%.0f°")
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_as_yaw_spd"),
                            1.f,
                            10.f,
                            _CFG("aim_as_yaw_spd").raw<float>(1.f),
                            0.1f,
                            STR("%.1fx")
                    )
            );

            auto scroll = new gui::scroll_layout(
                    JOAAT("aim_antiaim_standing_s"),
                    _VEC2(0.f, 35.f),
                    _VEC2(242.f, 355.f)
            );
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }

        return tab;
    }

    inline gui::tab* moving() {
        auto tab = new gui::content_tab(JOAAT("aim_antiaim_moving"), STR("MOVING"));
        {
            auto group = new gui::stack_layout(JOAAT("aim_antiaim_moving_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_am_pitch_t"), STR("Pitch")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_am_pitch"), {
                                    STR("Disabled"), STR("Up"), STR("Down"), STR("Fake up"), STR("Fake down")
                            }, _CFG("aim_am_pitch").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_am_yaw_t"), STR("Yaw")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_am_yaw"), {
                                    STR("Disabled"), STR("Forward"), STR("Backward"), STR("Jitter"), STR("Rotate")
                            }, _CFG("aim_am_yaw").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_am_yaw_add"),
                            -180.f,
                            180.f,
                            _CFG("aim_am_yaw_add").raw<float>(),
                            1.f,
                            STR("%.0f°")
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_am_yaw_mod_t"), STR("Modifier")));
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_am_yaw_mod"), 0.f, 360.f, _CFG("aim_am_yaw_mod").raw<float>(), 1.f, STR("%.0f°")
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_am_yaw_spd"),
                            1.f,
                            10.f,
                            _CFG("aim_am_yaw_spd").raw<float>(1.f),
                            0.1f,
                            STR("%.1fx")
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("aim_antiaim_moving_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }

        return tab;
    }

    inline gui::tab* air() {
        auto tab = new gui::content_tab(JOAAT("aim_antiaim_air"), STR("AIR"));
        {
            auto group = new gui::stack_layout(JOAAT("aim_antiaim_air_g"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_aa_pitch_t"), STR("Pitch")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_aa_pitch"), {
                                    STR("Disabled"), STR("Up"), STR("Down"), STR("Fake up"), STR("Fake down")
                            }, _CFG("aim_aa_pitch").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_aa_yaw_t"), STR("Yaw")));
            group->elements.emplace_back(
                    new gui::combobox(
                            JOAAT("aim_aa_yaw"), {
                                    STR("Disabled"), STR("Forward"), STR("Backward"), STR("Jitter"), STR("Rotate")
                            }, _CFG("aim_aa_yaw").raw<_BITS>(0)
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_aa_yaw_add"),
                            -180.f,
                            180.f,
                            _CFG("aim_aa_yaw_add").raw<float>(),
                            1.f,
                            STR("%.0f°")
                    )
            );
            group->elements.emplace_back(new gui::text_label(JOAAT("aim_aa_yaw_mod_t"), STR("Modifier")));
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_aa_yaw_mod"), 0.f, 360.f, _CFG("aim_aa_yaw_mod").raw<float>(), 1.f, STR("%.0f°")
                    )
            );
            group->elements.emplace_back(
                    new gui::slider(
                            JOAAT("aim_aa_yaw_spd"),
                            1.f,
                            10.f,
                            _CFG("aim_aa_yaw_spd").raw<float>(1.f),
                            0.1f,
                            STR("%.1fx")
                    )
            );

            auto scroll = new gui::scroll_layout(JOAAT("aim_antiaim_air_s"), _VEC2(0.f, 35.f), _VEC2(242.f, 355.f));
            scroll->elements.emplace_back(group);
            tab->elements.emplace_back(scroll);
        }

        return tab;
    }
}
namespace subtab_fakelag {
    inline gui::tab* standing() {
        auto tab = new gui::content_tab(JOAAT("aim_fakelag_standing"), STR("STANDING"));
        tab->is_selected = true;

        return tab;
    }

    inline gui::tab* moving() {
        auto tab = new gui::content_tab(JOAAT("aim_fakelag_moving"), STR("MOVING"));
        return tab;
    }

    inline gui::tab* air() {
        auto tab = new gui::content_tab(JOAAT("aim_fakelag_air"), STR("AIR"));
        return tab;
    }
}
namespace tab_aimbot {
    inline void group_general(gui::tab* tab) {
        auto group = new gui::stack_layout(JOAAT("aim_main"), _VEC2(0.f, 0.f), _VEC2(242.f, 0.f));
        group->elements.emplace_back(
                menu::make_line(
                        JOAAT("aim_enable_layout"), {
                                new gui::checkbox(
                                        JOAAT("aim_enable"),
                                        STR("Enable"),
                                        _CFG("aim_enable").raw<bool>(false)
                                ),
                                new gui::hotkey(
                                        JOAAT("aim_enable_k"),
                                        _CFG("aim_enable_k").raw<uint32_t>(0),
                                        _CFG("aim_enable_b").raw<uint32_t>(0)
                                )
                        }
                )
        );
        group->elements.emplace_back(new gui::text_label(JOAAT("aim_hb_text"), STR("Hitboxes")));
        group->elements.emplace_back(
                new gui::combobox(
                        JOAAT("aim_hb"), {
                                STR("Head"), STR("Chest"), STR("Pelvis"), STR("Arms"), STR("Legs"), STR("Feet")
                        }, _CFG("aim_hb").raw<_BITS>(0), true
                )
        );
        group->elements.emplace_back(new gui::text_label(JOAAT("aim_fov_text"), STR("FOV")));
        group->elements
             .emplace_back(
                     new gui::slider(
                             JOAAT("aim_fov"),
                             1.f,
                             180.f,
                             _CFG("aim_fov").raw<float>(180.f),
                             1.f,
                             STR("%.0f°")
                     )
             );
        group->elements
             .emplace_back(new gui::checkbox(JOAAT("aim_silent"), STR("Silent"), _CFG("aim_silent").raw<bool>(false)));
        group->elements
             .emplace_back(
                     new gui::checkbox(
                             JOAAT("aim_autofire"),
                             STR("Auto fire"),
                             _CFG("aim_autofire").raw<bool>(false)
                     )
             );

        auto _ = menu::make_group(JOAAT("aim_main"), STR("GENERAL"), _VEC2(20.f, 70.f), _VEC2(260.f, 400.f), group);
        tab->elements.emplace_back(_);
    }

    inline void group_antiaim(gui::tab* tab) {
        auto group = new gui::layout(JOAAT("aim_antiaim"), _VEC2(0.f, 0.f), _VEC2(242.f, 360.f));
        group->elements.emplace_back(subtab_antiaim::standing());
        group->elements.emplace_back(subtab_antiaim::moving());
        group->elements.emplace_back(subtab_antiaim::air());

        auto _ = menu::make_group(
                JOAAT("aim_antiaim"),
                STR("ANTI-AIMS"),
                _VEC2(295.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }

    inline void group_fakelag(gui::tab* tab) {
        auto group = new gui::layout(JOAAT("aim_fakelag"), _VEC2(0.f, 0.f), _VEC2(242.f, 360.f));
        group->elements.emplace_back(subtab_fakelag::standing());
        group->elements.emplace_back(subtab_fakelag::moving());
        group->elements.emplace_back(subtab_fakelag::air());

        auto _ = menu::make_group(
                JOAAT("aim_fakelag"),
                STR("FAKE LAGS"),
                _VEC2(570.f, 70.f),
                _VEC2(260.f, 400.f),
                group
        );
        tab->elements.emplace_back(_);
    }
}

gui::title_tab* menu::make_tab_aim() {
    auto tab = new gui::title_tab(JOAAT("aim"), STR("AIMBOT"), STR("B"));
    tab_aimbot::group_general(tab);
    tab_aimbot::group_antiaim(tab);
    tab_aimbot::group_fakelag(tab);

    return tab;
}