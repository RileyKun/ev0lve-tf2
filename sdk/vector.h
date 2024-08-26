//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_VECTOR_H
#define EV0LVE_TF_VECTOR_H

namespace sdk {
    class vec3 {
    public:
        vec3() = default;
        vec3(float _x, float _y) { x = _x; y = _y; }
        vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

        inline vec3 operator+(const vec3& o) const { return vec3(x + o.x, y + o.y, z + o.z); }
        inline vec3 operator-(const vec3& o) const { return vec3(x - o.x, y - o.y, z - o.z); }
        inline vec3 operator/(const vec3& o) const { return vec3(x / o.x, y / o.y, z / o.z); }
        inline vec3 operator*(const vec3& o) const { return vec3(x * o.x, y * o.y, z * o.z); }
        inline vec3 operator+(const float o) const { return vec3(x + o, y + o, z + o); }
        inline vec3 operator-(const float o) const { return vec3(x - o, y - o, z - o); }
        inline vec3 operator/(const float o) const { return vec3(x / o, y / o, z / o); }
        inline vec3 operator*(const float o) const { return vec3(x * o, y * o, z * o); }

        inline void operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; }
        inline void operator-=(const vec3& o) { x -= o.x; y -= o.y; z -= o.z; }
        inline void operator/=(const vec3& o) { x /= o.x; y /= o.y; z /= o.z; }
        inline void operator*=(const vec3& o) { x *= o.x; y *= o.y; z *= o.z; }
        inline void operator+=(const float o) { x += o; y += o; z += o; }
        inline void operator-=(const float o) { x -= o; y -= o; z -= o; }
        inline void operator/=(const float o) { x /= o; y /= o; z /= o; }
        inline void operator*=(const float o) { x *= o; y *= o; z *= o; }

        inline bool operator!=(const vec3& o) const { return x != o.x || y != o.y || z != o.z; }
        inline bool operator!=(const float o) const { return x != o || y != o || z != o; }
        inline bool operator==(const vec3& o) const { return x == o.x && y == o.y && z == o.z; }
        inline bool operator==(const float o) const { return x == o && y == o && z == o; }

        [[nodiscard]] inline float dot(const vec3& o) const { return x * o.x + y * o.y + z * o.z; }
        [[nodiscard]] inline float dot(const float o) const { return x * o + y * o + z * o; }
        [[nodiscard]] inline float dist(const vec3& o) const { return (*this - o).len(); }

        [[nodiscard]] inline float len_sqr() const { return x * x + y * y + z * z; }
        [[nodiscard]] inline float len() const { return sqrtf(x * x + y * y + z * z); }
        [[nodiscard]] inline float len_2d_sqr() const { return x * x + y * y; }
        [[nodiscard]] inline float len_2d() const { return sqrtf(x * x + y * y); }

        inline vec3& normalize() {
            auto l = len();
            if (l > 0.f) {
                x /= l;
                y /= l;
                z /= l;
            }

            return *this;
        }

        inline vec3 transform(const matrix3x4& in) {
            vec3 out;
            out.x = x * in.mtx[0][0] + y * in.mtx[0][1] + z * in.mtx[0][2] + in.mtx[0][3];
            out.y = x * in.mtx[1][0] + y * in.mtx[1][1] + z * in.mtx[1][2] + in.mtx[1][3];
            out.z = x * in.mtx[2][0] + y * in.mtx[2][1] + z * in.mtx[2][2] + in.mtx[2][3];

            return out;
        }

        float x{}, y{}, z{};
    };

    class __declspec(align(16)) vec_aligned : public vec3 {
    public:
        vec_aligned() : vec3(){}
        vec_aligned(const vec3& v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }

        float w{};
    };

    class ang {
    public:
        ang() = default;
        ang(float _p, float _y) { p = _p; y = _y; }
        ang(float _p, float _y, float _r) { p = _p; y = _y; r = _r; }

        ang(const vec3& a, const vec3& b) {
            auto d = a - b;
            p = asinf(d.z / d.len()) * 57.295779513082f;
            y = atanf(d.y / d.x) * 57.295779513082f;

            if (d.x >= 0.f)
                y += 180.f;
        }

        inline ang operator+(const ang& o) const { return ang(p + o.p, y + o.y, r + o.r); }
        inline ang operator-(const ang& o) const { return ang(p - o.p, y - o.y, r - o.r); }
        inline ang operator/(const ang& o) const { return ang(p / o.p, y / o.y, r / o.r); }
        inline ang operator*(const ang& o) const { return ang(p * o.p, y * o.y, r * o.r); }
        inline ang operator+(const float o) const { return ang(p + o, y + o, r + o); }
        inline ang operator-(const float o) const { return ang(p - o, y - o, r - o); }
        inline ang operator/(const float o) const { return ang(p / o, y / o, r / o); }
        inline ang operator*(const float o) const { return ang(p * o, y * o, r * o); }

        inline void operator+=(const ang& o) { p += o.p; y += o.y; r += o.r; }
        inline void operator-=(const ang& o) { p -= o.p; y -= o.y; r -= o.r; }
        inline void operator/=(const ang& o) { p /= o.p; y /= o.y; r /= o.r; }
        inline void operator*=(const ang& o) { p *= o.p; y *= o.y; r *= o.r; }
        inline void operator+=(const float o) { p += o; y += o; r += o; }
        inline void operator-=(const float o) { p -= o; y -= o; r -= o; }
        inline void operator/=(const float o) { p /= o; y /= o; r /= o; }
        inline void operator*=(const float o) { p *= o; y *= o; r *= o; }

        inline bool operator!=(const ang& o) const { return p != o.p || y != o.y || r != o.r; }
        inline bool operator!=(const float o) const { return p != o || y != o || r != o; }
        inline bool operator==(const ang& o) const { return p == o.p && y == o.y && r == o.r; }
        inline bool operator==(const float o) const { return p == o && y == o && r == o; }

        inline void vectors(vec3& forward) const {
            float sp{}, sy{}, cp{}, cy{};

            math::sin_cos(math::rad(y), &sy, &cy);
            math::sin_cos(math::rad(p), &sp, &cp);

            forward.x = cp * cy;
            forward.y = cp * sy;
            forward.z = -sp;
        }

        inline void vectors(vec3& forward, vec3& right, vec3& up) const {
            float sp{}, sy{}, sr{}, cp{}, cy{}, cr{};

            math::sin_cos(math::rad(y), &sy, &cy);
            math::sin_cos(math::rad(p), &sp, &cp);
            math::sin_cos(math::rad(r), &sr, &cr);

            forward.x = cp * cy;
            forward.y = cp * sy;
            forward.z = -sp;
            right.x = -1.f * sr * sp * cy + -1.f * cr * -sy;
            right.y = -1.f * sr * sp * sy + -1.f * cr * cy;
            right.z = -1.f * sr * cp;
            up.x = cr * sp * cy + -sr * -sy;
            up.y = cr * sp * sy + -sr * cy;
            up.z = cr * cp;
        }

        inline ang& normalize() {
            p = fmodf(p, 180.f);
            y = fmodf(y, 360.f);
            r = 0.f;

            if (p > 90.f) p -= 180.f;
            if (p < -90.f) p += 180.f;
            if (y > 180.f) y -= 360.f;
            if (y < -180.f) y += 360.f;

            return *this;
        }

        inline float fov(ang o) {
            auto d = *this - o;
            d.normalize();

            return sqrtf(powf(d.p, 2.f) + powf(d.y, 2.f));
        }

        float p{}, y{}, r{};
    };
}

#endif //EV0LVE_TF_VECTOR_H
