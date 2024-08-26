//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_MENU_H
#define EV0LVE_TF_MENU_H

#include <gui/gui.h>

namespace menu {
    void init();
    void draw();
    void reset_colors();

    gui::title_tab* make_tab_misc();
    gui::title_tab* make_tab_vis();
    gui::title_tab* make_tab_aim();
    gui::title_tab* make_tab_lua();

    bool is_opened();
    void set_opened(bool b);

    gui::layout* make_group(rage::joaat_t id, const std::string& label, const _VEC2& pos, const _VEC2& size,  gui::layout* inner);
    gui::layout* make_line(rage::joaat_t id, const std::vector<gui::element*>& els);
}

#endif //EV0LVE_TF_MENU_H
