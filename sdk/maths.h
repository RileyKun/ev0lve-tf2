//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_MATHS_H
#define EV0LVE_TF_MATHS_H

namespace math {
    inline void sin_cos(float r, float* s, float* c) {
        *s = sinf(r);
        *c = cosf(r);
    }

    inline float rad(float d) {
        return d * ((float)M_PI / 180.f);
    }

    inline float deg(float r) {
        return r * (180.f / (float)M_PI);
    }
}

#endif //EV0LVE_TF_MATHS_H
