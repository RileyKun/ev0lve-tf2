//
// Created by panzerfaust on 5/26/2020.
//

#include <ev0lve.h>
#include <core/cfg.h>
#include <sdk/sdk.h>
#include <hacks/autojump.h>
#include <core/game.h>

void hack::c_autojump::work() {
    const auto mt = game::me->move_type();
    if (mt == sdk::mt_no_clip || mt == sdk::mt_ladder) {
        is_hopping = false;
        return;
    }

    if (_CFG("misc_autojump").get<bool>(false))
        auto_jump();
    if (_CFG("misc_auto_strafe").get<bool>(false))
        auto_strafe();
}

void hack::c_autojump::auto_jump() {
    static bool disable_hopping = false;
    if (game::cmd->buttons & sdk::in_jump) {
        if (game::cmd->buttons & sdk::in_duck)
            disable_hopping = true;
    } else
        disable_hopping = false;

    is_hopping = ((game::cmd->buttons & sdk::in_jump) || !(game::me->flags() & sdk::fl_on_ground)) && !disable_hopping;
    if (game::cmd->buttons & sdk::in_jump && !disable_hopping) {
        if (!(game::me->flags() & sdk::fl_on_ground))
            game::cmd->buttons &= ~sdk::in_jump;
    }
}

void hack::c_autojump::auto_strafe() const {
    if (!is_hopping)
        return;

    auto cmd = game::cmd;
    auto wish_ang = cmd->view_angles;
    if (cmd->buttons & sdk::in_back)
        wish_ang.y -= 180.f;
    if (cmd->buttons & sdk::in_move_right)
        wish_ang.y -= (cmd->buttons & sdk::in_back) ? -45.f : ((cmd->buttons & sdk::in_forward) ? 45.f : 90.f);
    if (cmd->buttons & sdk::in_move_left)
        wish_ang.y += (cmd->buttons & sdk::in_back) ? -45.f : ((cmd->buttons & sdk::in_forward) ? 45.f : 90.f);

    if (abs(cmd->mouse_dx) > 2) {
        cmd->side_move = (cmd->mouse_dx < 0) ? -450.f : 450.f;
        cmd->forward_move = 0;
        cmd->fix_movement(wish_ang);

        return;
    }

    if (game::me->velocity().len_2d() <= 0.5f) {
        cmd->forward_move = 450.f;
        cmd->fix_movement(wish_ang);

        return;
    }

    const auto vel = game::me->velocity();
    const auto diff = sdk::ang(0.f, wish_ang.y - math::deg(atan2f(vel.y, vel.x)), 0.f).normalize().y;

    wish_ang.y = sdk::ang(0, wish_ang.y - diff * 0.8f, 0.f).normalize().y;

    cmd->forward_move = std::clamp((5850.f / game::me->velocity().len_2d()), -450.f, 450.f);
    cmd->side_move = (diff > 0.f) ? -450.f : 450.f;
    cmd->fix_movement(wish_ang);
}
