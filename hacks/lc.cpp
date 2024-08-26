//
// Created by panzerfaust on 6/7/2020.
//

#include <misc/xor.h>
#include <core/cfg.h>
#include <misc/logger.h>
#include <core/game.h>
#include "lc.h"

void hack::c_lc::work() {
    for (auto i = 1; i < sdk::global_vars->max_clients; i++) {
        auto e = sdk::entity_list->get(i);
        if (!e || e->team() == game::me->team() || e->life_state() != sdk::ls_alive || e->is_dormant()) {
            if (!records[i].empty())
                records[i].clear();

            continue;
        }

        populate(e);
    }
}

hack::lc_record hack::c_lc::get_last(sdk::entity *ent) {
    auto& r = records[ent->index()];
    return r.empty() ? lc_record{} : r.back();
}

hack::lc_record hack::c_lc::get_first(sdk::entity *ent) {
    auto& r = records[ent->index()];
    return r.empty() ? lc_record{} : r.front();
}

std::vector<hack::lc_record>& hack::c_lc::get(sdk::entity *ent) {
    return records[ent->index()];
}

void hack::c_lc::populate(sdk::entity *ent) {
    auto& v = records[ent->index()];
    for (auto i = v.begin(); i != v.end(); ) {
        if (!is_valid(*i))
            i = v.erase(i);
        else
            ++i;
    }

    lc_record r{};
    r.entity = ent;
    r.pos = ent->get_abs_origin();
    r.sim_time = ent->sim_time();

    if (!v.empty())
        r.id = v.back().id + 1;
    if (!ent->setup_bones(r.bones, 128, 0x7FF00, sdk::global_vars->server_time))
        memset(r.bones, 0, sizeof(sdk::matrix3x4) * 128);

    v.emplace_back(r);
}

bool hack::c_lc::is_valid(lc_record& r) {
    if (!sdk::client_state->channel)
        return false;

    auto correct = std::clamp(get_lerp_time() + sdk::client_state->channel->get_latency(0), 0.f, 1.f);
    return r.entity->sim_time() - r.sim_time <= correct;
}

float hack::c_lc::get_lerp_time() {
    static auto cl_interp = sdk::cvar->find(STR("cl_interp"));
    static auto cl_interp_ratio = sdk::cvar->find(STR("cl_interp_ratio"));
    static auto cl_updaterate = sdk::cvar->find(STR("cl_updaterate"));

    return max(cl_interp->get_float(), cl_interp_ratio->get_float() / cl_updaterate->get_float());
}

