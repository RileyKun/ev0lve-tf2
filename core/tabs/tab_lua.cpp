//
// Created by panzerfaust on 6/5/2020.
//

#include <core/cfg.h>
#include <core/menu.h>
#include <misc/xor.h>

namespace tab_lua {
}

gui::title_tab* menu::make_tab_lua() {
    auto tab = new gui::title_tab(JOAAT("lua"), STR("SCRIPTING"), STR("G"));


    return tab;
}