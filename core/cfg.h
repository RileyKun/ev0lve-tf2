//
// Created by panzerfaust on 5/10/2020.
//

#ifndef EV0LVE_CFGSYSTEM_CFG_H
#define EV0LVE_CFGSYSTEM_CFG_H

#include <ev0lve.h>
#include <misc/joaat.h>

namespace cfg {
    class config_value {
    public:
        template<typename T>
        T get(std::optional<T> def = std::nullopt) {
            if (!init)
                return def.value_or(T{});

            T tmp{};
            memcpy(&tmp, mem, sizeof(T));
            return tmp;
        }

        template<typename T>
        void set(T v, size_t custom_size = 0) {
            set(&v, custom_size);
        }

        template<typename T>
        void set(T* v, size_t custom_size = 0) {
            if (!init) {
                size = custom_size != 0 ? custom_size : sizeof(T);
                memset(mem, 0, 64);

                init = true;
            }

            memcpy(mem, v, size);
        }

        template<typename T>
        T* raw(std::optional<T> def = std::nullopt) {
            if (!init) {
                set(def.value_or(T{}));
                memcpy(this->def, mem, 64);
            }

            return reinterpret_cast<T*>(mem);
        }

        void reset() {
            if (init)
                memcpy(mem, def, 64);
        }

        void invalidate() {
            if (init) {
                init = false;
                memset(mem, 0, 64);
                memset(def, 0, 64);
            }
        }

        char mem[64]{};
        char def[64]{};
        size_t size = 0;
        bool init = false;
    };

    enum class config_result {
        OK,
        ERROR_BAD_FILE,
        ERROR_BAD_MAGIC,
        ERROR_BAD_CHEAT_ID,
        ERROR_BAD_VERSION
    };

    class config_system {
    public:
        config_result save(const std::string& filename);
        config_result load(const std::string& filename, bool should_reset = true);
        static void reset();
        static void destroy();

        std::vector<std::string>& refresh(const std::string& ext = ".dat");

        // Path to folder with config files
        std::string path;
        std::vector<std::string> files;

        short magic = 0;
        rage::joaat_t cheat_id = 0;
        unsigned int version = 0;
    };

    inline config_system* ctx = new config_system;
    inline std::unordered_map<rage::joaat_t, config_value> storage;
}

#define _CFG(v) cfg::storage[JOAAT(v)]

#endif //EV0LVE_CFGSYSTEM_CFG_H
