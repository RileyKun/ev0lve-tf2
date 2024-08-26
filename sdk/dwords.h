//
// Created by panzerfaust on 6/8/2020.
//

#ifndef EV0LVE_TF_DWORDS_H
#define EV0LVE_TF_DWORDS_H

namespace sdk {
    template <typename T>
    inline T dword_swap(T dw)
    {
        uint32_t temp;

        temp  =   *((uint32_t *)&dw) 				>> 24;
        temp |= ((*((uint32_t *)&dw) & 0x00FF0000) >> 8);
        temp |= ((*((uint32_t *)&dw) & 0x0000FF00) << 8);
        temp |= ((*((uint32_t *)&dw) & 0x000000FF) << 24);

        return *((T*)&temp);
    }

    inline uint32_t little_dword(uint32_t val) { int test = 1; return ( *(char *)&test == 1 ) ? val : dword_swap( val ); }
}

#endif //EV0LVE_TF_DWORDS_H
