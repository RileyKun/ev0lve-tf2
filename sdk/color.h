//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_COLOR_H
#define EV0LVE_TF_COLOR_H

namespace sdk {
    class color {
    public:
        color() = default;
        color(int _r, int _g, int _b) { r = _r; g = _g; b = _b; a = 255; }
        color(int _r, int _g, int _b, int _a) { r = _r; g = _g; b = _b; a = _a; }

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
    };
}

#endif //EV0LVE_TF_COLOR_H
