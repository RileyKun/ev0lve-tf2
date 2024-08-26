//
// Created by panzerfaust on 5/27/2020.
//

#ifndef EV0LVE_TF_CRC32_H
#define EV0LVE_TF_CRC32_H

#include <ev0lve.h>

namespace crc32 {
    void init(uint32_t* crc);
    void process_buffer(uint32_t* crc, void* p, int len);
    void final(uint32_t* crc);
}

#endif //EV0LVE_TF_CRC32_H
