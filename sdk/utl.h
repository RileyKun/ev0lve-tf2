//
// Created by panzerfaust on 5/27/2020.
//

#ifndef EV0LVE_TF_UTL_H
#define EV0LVE_TF_UTL_H

namespace sdk {
    class utl_string {
    public:
        operator std::string() { return string; }
        operator const char*() { return string; }

        char* string;
    };

    class utl_blob {
    public:
        utl_blob*   prev;
        utl_blob*   next;
        int         num_bytes;
        char        data[1];
        char        padding[3];
    };

    class utl_mem_pool {
    public:
        int             block_size;
        int             blocks_per_blob;
        int             grow_mode;
        int             blocks_allowed;
        void*           head_of_free_list;
        int             peak_alloc;
        uint16_t        alignment;
        uint16_t        blobs_count;
        const char*     owner;
        utl_blob        head;
    };

    class utl_memory {
    public:
        void*   memory;
        int     allocation_count;
        int     grow_size;
    };

    class utl_vector {
    public:
        utl_memory  memory;
        int         size;
        void*       elements;
    };

    class utl_linked_list_64 {
    public:
        utl_memory  memory;
        uint64_t    head;
        uint64_t    tail;
        uint64_t    first_free;
        uint64_t    element_count;
        uint64_t    num_allocated;
        uint32_t    last_alloc;
        void*       elements;
    };
}

#endif //EV0LVE_TF_UTL_H
