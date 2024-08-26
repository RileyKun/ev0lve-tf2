//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_NETVARS_H
#define EV0LVE_TF_NETVARS_H

namespace sdk {
    class netvar {
    public:
        std::string name{};
        uint32_t offset{};
        prop_type type{};

        std::unordered_map<uint32_t, netvar> children;

        inline netvar& operator[](uint32_t k) {
            return children[k];
        }

        inline netvar add(uint32_t c) {
            netvar n = *this;
            n.offset += c;
            return n;
        }

        inline netvar sub(uint32_t c) {
            netvar n = *this;
            n.offset -= c;
            return n;
        }

        template<typename T>
        inline T& get(void* o, uint32_t c = 0) {
            return *reinterpret_cast<T*>(reinterpret_cast<uint32_t>(o) + offset + c * sizeof(T));
        }
    };

    class datatable {
    public:
        std::string name{};
        std::unordered_map<uint32_t, netvar> children;

        inline netvar& operator[](uint32_t k) {
            return children[k];
        }
    };

    inline std::unordered_map<uint32_t, datatable> netvars;
}

#endif //EV0LVE_TF_NETVARS_H
