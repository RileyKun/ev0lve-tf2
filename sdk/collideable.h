//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_COLLIDEABLE_H
#define EV0LVE_TF_COLLIDEABLE_H

namespace sdk {
    class collideable {
    public:
        inline const vec3& mins() {
            return hack::virtual_func<const vec3&(__thiscall*)(void*)>(this, 3)(this);
        }

        inline const vec3& maxs() {
            return hack::virtual_func<const vec3&(__thiscall*)(void*)>(this, 4)(this);
        }
    };
}

#endif //EV0LVE_TF_COLLIDEABLE_H
