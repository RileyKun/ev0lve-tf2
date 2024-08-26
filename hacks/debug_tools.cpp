//
// Created by panzerfaust on 5/30/2020.
//

#include "debug_tools.h"
#include <renderer/renderer.h>
#include <sdk/sdk.h>

void hack::c_debug_tools::draw() {
    _VEC2 offset;
    offset.x = 10.f;
    offset.y = 200.f;

    for (auto& g : graphs) {
        g->draw(offset);
        offset.y += 60.f;
    }
}

hack::graph::graph(const std::string &name, float min, float max, float rate) {
    this->name = name;
    this->min = min;
    this->max = max;
    this->rate = rate;
}

void hack::graph::draw(const types::vec2 &pos) {
    const auto size = _VEC2(250.f, 50.f);

    auto d = &renderer::adapter->foreground;
    d->push_clip_rect(_RECT(pos - _VEC2(0.f, 1.f), pos + size));
    d->add_rect_filled(_RECT(pos, pos + size), _COLOR(0, 0, 0, 150));
    d->add_text(JOAAT("main"), name, pos + _VEC2(5.f, -6.f), _COLOR(255, 255, 255));

    if (!values.empty()) {
        auto val_size = renderer::tools::text_size(JOAAT("main"), std::to_string(values.back()));
        d->add_text(JOAAT("main"), std::to_string(values.back()), pos + _VEC2(size.x - val_size.x - 5.f, -6.f), _COLOR(255, 255, 255));
    }

    if (values.size() > 250 * rate + 1)
        values.erase(values.begin());

    auto prev = pos;
    for (auto& v : values) {
        v = std::clamp(v, min, max);

        if (prev == pos) {
            prev.y += 50.f - ((v - min) / (max - min) * 35.f);
            continue;
        } else {
            auto o_prev = prev;
            prev.x += 1.f / rate;
            prev.y = pos.y + 50.f - ((v - min) / (max - min) * 35.f);

            d->add_line(o_prev, prev, _COLOR(255, 255, 255), 2.f);
        }
    }

    d->add_rect(_RECT(pos, pos + size), _COLOR(0x00, 0xAC, 0xF5));
    d->pop_clip_rect();
}

void hack::graph::add(float v) {
    if (sdk::global_vars->realtime - last_ins_time >= rate) {
        values.emplace_back(v);
        last_ins_time = sdk::global_vars->realtime;
    }
}
