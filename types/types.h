//
// Created by panzerfaust on 5/18/2020.
//

#ifndef EV0LVE_RENDERER_TYPES_H
#define EV0LVE_RENDERER_TYPES_H

#include <cmath>
#include <algorithm>

namespace types {
    class vec2 {
    public:
        vec2() = default;
        vec2(float _x, float _y) {
            x = _x;
            y = _y;
        }

        bool operator==(const vec2& o) const { return x == o.x && y == o.y; }
        bool operator==(const float o) const { return x == o && y == o; }
        bool operator!=(const vec2& o) const { return x != o.x || y != o.y; }
        bool operator!=(const float o) const { return x != o || y != o; }

        vec2 operator+(const vec2& o) const { return vec2(x + o.x, y + o.y); }
        vec2 operator-(const vec2& o) const { return vec2(x - o.x, y - o.y); }
        vec2 operator/(const vec2& o) const { return vec2(x / o.x, y / o.y); }
        vec2 operator*(const vec2& o) const { return vec2(x * o.x, y * o.y); }
        vec2 operator+(const float o) const { return vec2(x + o, y + o); }
        vec2 operator-(const float o) const { return vec2(x - o, y - o); }
        vec2 operator/(const float o) const { return vec2(x / o, y / o); }
        vec2 operator*(const float o) const { return vec2(x * o, y * o); }

        void operator+=(const vec2& o) { x += o.x; y += o.y; }
        void operator-=(const vec2& o) { x -= o.x; y -= o.y; }
        void operator/=(const vec2& o) { x /= o.x; y /= o.y; }
        void operator*=(const vec2& o) { x *= o.x; y *= o.y; }
        void operator+=(float o) { x += o; y += o; }
        void operator-=(float o) { x -= o; y -= o; }
        void operator/=(float o) { x /= o; y /= o; }
        void operator*=(float o) { x *= o; y *= o; }

        inline vec2& floor() { x = floorf(x); y = floorf(y); return *this; }
        inline vec2& ceil() { x = ceilf(x); y = ceilf(y); return *this; }
        inline vec2& round() { x = roundf(x); y = roundf(y); return *this; }

        inline float len_sqr() const { return x * x + y * y; }
        inline float len() const { return sqrtf(x * x + y * y); }

        float x = 0.f;
        float y = 0.f;
    };

    class vec4 {
    public:
        vec4() = default;
        vec4(const vec2 &a, const vec2 &b) {
            x = a.x;
            y = a.y;
            z = b.x;
            w = b.y;
        }
        vec4(float _x, float _y, float _z, float _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
        float w = 0.f;
    };

    class rect {
    public:
        rect() = default;
        rect(const vec2 &a, const vec2 &b) {
            mins = a;
            maxs = b;
        }
        rect(const vec4 &v) {
            mins = vec2(v.x, v.y);
            maxs = vec2(v.w, v.z);
        }
        rect(float x1, float y1, float x2, float y2) {
            mins = vec2(x1, y1);
            maxs = vec2(x2, y2);
        }

        inline rect& translate(const vec2& v) { mins += v; maxs += v; return *this; }

        inline rect shrink(float f) const { return rect(mins + f, maxs - f); }
        inline rect expand(float f) const { return shrink(-f); }

        inline float width() const { return maxs.x - mins.x; }
        inline float height() const { return maxs.y - mins.y; }

        inline rect intersect(const rect& o) const {
            return rect(
                    o.mins.x < mins.x ? mins.x : o.mins.x,
                    o.mins.y < mins.y ? mins.y : o.mins.y,
                    o.maxs.x > maxs.x ? maxs.x : o.maxs.x,
                    o.maxs.y > maxs.y ? maxs.y : o.maxs.y
            );
        }

        inline vec2 tl() const { return mins; }
        inline vec2 tr() const { return vec2(maxs.x, mins.y); }
        inline vec2 br() const { return maxs; }
        inline vec2 bl() const { return vec2(mins.x, maxs.y); }
        inline vec2 middle() const { return vec2(mins.x + width() * 0.5f, mins.y + height() * 0.5f); }
        inline vec2 circular(float r) const { return middle() + vec2(width() * 0.5f * cosf(r), height() * 0.5f * sinf(r)); }

        inline bool contains(const vec2& p) const { return p.x >= mins.x && p.x <= maxs.x && p.y >= mins.y && p.y <= maxs.y; }
        inline bool contains(const rect& p) const { return p.mins.x >= mins.x && p.maxs.x <= maxs.x && p.mins.y >= mins.y && p.maxs.y <= maxs.y; }

        vec2 mins;
        vec2 maxs;
    };

    class color {
    public:
        color() = default;
        color(int r, int g, int b, int a = 255) {
            value.x = (float)r / 255.f;
            value.y = (float)g / 255.f;
            value.z = (float)b / 255.f;
            value.w = (float)a / 255.f;
        }
        color(float r, float g, float b, float a = 1.f) {
            value = types::vec4(r, g, b, a);
        }
        color(uint32_t c) {
            value.x = (c >> 0U & 0xFFU) / 255.f;
            value.y = (c >> 8U & 0xFFU) / 255.f;
            value.z = (c >> 16U & 0xFFU) / 255.f;
            value.w = (c >> 24U & 0xFFU) / 255.f;
        }
        color(const vec4 &v) {
            value = v;
        }

        operator vec4() const {
            return value;
        }

        operator uint32_t() const {
            return  (uint32_t)(value.x * 255.f) << 0U |
                    (uint32_t)(value.y * 255.f) << 8U |
                    (uint32_t)(value.z * 255.f) << 16U |
                    (uint32_t)(value.w * 255.f) << 24U;
        }

        inline color mod_a(uint32_t a) { clamp(); return color(r(), g(), b(), std::clamp(a, 0U, (uint32_t)this->a())); }
        inline color mod_a(float a) { clamp(); return color(value.x, value.y, value.z, std::clamp(a, 0.f, value.w)); }
        inline color mod_a(uint8_t a) { clamp(); return mod_a((uint32_t)a); }

        inline color clamp_a(float a) const { return color(value.x, value.y, value.z, std::clamp(value.w, 0.f, a)); }

        inline uint8_t r() { clamp(); return (uint8_t)(value.x * 255.f); }
        inline uint8_t g() { clamp(); return (uint8_t)(value.y * 255.f); }
        inline uint8_t b() { clamp(); return (uint8_t)(value.z * 255.f); }
        inline uint8_t a() { clamp(); return (uint8_t)(value.w * 255.f); }
        inline void r(uint8_t v) { value.x = v / 255.f; clamp(); }
        inline void g(uint8_t v) { value.y = v / 255.f; clamp(); }
        inline void b(uint8_t v) { value.z = v / 255.f; clamp(); }
        inline void a(uint8_t v) { value.w = v / 255.f; clamp(); }

        inline uint16_t h() {
            clamp();

            auto _max = fmax(value.x, fmax(value.y, value.z));
            auto _min = fmin(value.x, fmin(value.y, value.z));
            auto d = _max - _min;

            auto _h = 0U;
            if (_max == _min)
                _h = 0U;
            if (_max == value.x)
                _h = (uint16_t)(60 * ((value.y - value.z) / d) + 360) % 360;
            if (_max == value.y)
                _h = (uint16_t)(60 * ((value.z - value.x) / d) + 120) % 360;
            if (_max == value.z)
                _h = (uint16_t)(60 * ((value.x - value.y) / d) + 240) % 360;

            return _h;
        }

        inline float s() {
            clamp();

            auto _max = fmax(value.x, fmax(value.y, value.z));
            auto _min = fmin(value.x, fmin(value.y, value.z));
            auto d = _max - _min;

            return _max != 0.f ? d / _max : 0.f;
        }

        inline float v() {
            clamp();

            return fmax(value.x, fmax(value.y, value.z));
        }

        inline color& hsv(uint16_t _h, float _s, float _v, float a = -1.f) {
            auto hi = ((uint32_t)floorf(_h / 60.f)) % 6;
            auto f = _h / 60.f - floorf(_h / 60.f);

            auto _a = (int)(_v * 255.f);
            auto _p = (int)(_a * (1.f - _s));
            auto _q = (int)(_a * (1.f - f * _s));
            auto _t = (int)(_a * (1.f - (1.f - f) * _s));

            if (hi == 0) value = vec4(_a / 255.f, _t / 255.f, _p / 255.f, value.w);
            if (hi == 1) value = vec4(_q / 255.f, _a / 255.f, _p / 255.f, value.w);
            if (hi == 2) value = vec4(_p / 255.f, _a / 255.f, _t / 255.f, value.w);
            if (hi == 3) value = vec4(_p / 255.f, _q / 255.f, _a / 255.f, value.w);
            if (hi == 4) value = vec4(_t / 255.f, _p / 255.f, _a / 255.f, value.w);
            if (hi == 5) value = vec4(_a / 255.f, _p / 255.f, _q / 255.f, value.w);

            if (a > -1.f)
                value.w = a;

            clamp();
            return *this;
        }

        inline void clamp() {
            value.x = std::clamp(value.x, 0.f, 1.f);
            value.y = std::clamp(value.y, 0.f, 1.f);
            value.z = std::clamp(value.z, 0.f, 1.f);
            value.w = std::clamp(value.w, 0.f, 1.f);
        }

        vec4 value;
    };

    class bits {
    public:
        bits() = default;
        bits(uint32_t v) { value = v; }

        operator uint32_t() const { return value; }

        inline void set(uint32_t n) { value |= 1 << n; }
        inline void unset(uint32_t n) { value &= ~(1 << n); }
        inline bool get(uint32_t n) { return (value >> n) & 1; }

        uint32_t value{};
    };
}

#define _VEC2 types::vec2
#define _VEC4 types::vec4
#define _RECT types::rect
#define _COLOR types::color
#define _BITS types::bits

#define _COLOR_WHITE _COLOR(255, 255, 255)
#define _COLOR_BLACK _COLOR(0, 0, 0)

#endif //EV0LVE_RENDERER_TYPES_H
