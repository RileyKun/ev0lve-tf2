//
// Created by panzerfaust on 6/1/2020.
//

#ifndef EV0LVE_TF_RENDERVIEW_H
#define EV0LVE_TF_RENDERVIEW_H

namespace sdk {
    class c_render_view {
    public:
        inline void set_blend(float v) {
            hack::virtual_func<void(__thiscall*)(void*, float)>(this, 4)(this, v);
        }

        inline void set_color(float* v) {
            hack::virtual_func<void(__thiscall*)(void*, const float*)>(this, 6)(this, v);
        }

        inline void set_color(const color& c) {
            set_color(new float[3]{ (float)c.r / 255.f, (float)c.g / 255.f, (float)c.b / 255.f });
            set_blend((float)c.a / 255.f);
        }
    };

    inline c_render_view* render_view = nullptr;
}

#endif //EV0LVE_TF_RENDERVIEW_H
