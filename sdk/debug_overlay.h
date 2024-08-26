//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_DEBUG_OVERLAY_H
#define EV0LVE_TF_DEBUG_OVERLAY_H

namespace sdk {
    class c_debug_overlay {
    public:
        inline void add_box(const vec3& p, const vec3& mins, const vec3& maxs, const ang& r, const color& c, float d) {
            hack::virtual_func<void(__thiscall*)(void*, const vec3&, const vec3&, const vec3&, const ang&, int, int, int, int, float)>
                    (this, 1)
                    (this, p, mins, maxs, r, c.r, c.g, c.b, c.a, d);
        }

        inline void add_line(const vec3& p, const vec3& d, const color& c, bool depth, float dur) {
            hack::virtual_func<void(__thiscall*)(void*, const vec3&, const vec3&, int, int, int, bool, float)>(this, 3)
                    (this, p, d, c.r, c.g, c.b, depth, dur);
        }

        inline int screen_position(const vec3& w, vec3& s) {
            return hack::virtual_func<int(__thiscall*)(void*, const vec3&, vec3&)>(this, 10)
                    (this, w, s);
        }
    };

    inline c_debug_overlay* debug_overlay = nullptr;
}

#endif //EV0LVE_TF_DEBUG_OVERLAY_H
