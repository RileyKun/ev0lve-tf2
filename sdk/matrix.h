//
// Created by panzerfaust on 6/2/2020.
//

#ifndef EV0LVE_TF_MATRIX_H
#define EV0LVE_TF_MATRIX_H

namespace sdk {
    class matrix3x4 {
    public:
        inline float* operator[](int i) { return mtx[i]; }

        float mtx[3][4]{};
    };
}

#endif //EV0LVE_TF_MATRIX_H
