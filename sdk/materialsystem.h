//
// Created by panzerfaust on 6/1/2020.
//

#ifndef EV0LVE_TF_MATERIALSYSTEM_H
#define EV0LVE_TF_MATERIALSYSTEM_H

namespace sdk {
    class material_var {
    public:
        inline void set_float(float v) {
            hack::virtual_func<void(__thiscall*)(void*, float)>(this, 3)(this, v);
        }

        inline void set_int(int v) {
            hack::virtual_func<void(__thiscall*)(void*, int)>(this, 4)(this, v);
        }

        inline void set_string(const std::string& v) {
            hack::virtual_func<void(__thiscall*)(void*, const char*)>(this, 5)(this, v.c_str());
        }

        inline void set_vec(float x, float y) {
            hack::virtual_func<void(__thiscall*)(void*, float, float)>(this, 10)(this, x, y);
        }

        inline void set_vec(float x, float y, float z) {
            hack::virtual_func<void(__thiscall*)(void*, float, float, float)>(this, 11)(this, x, y, z);
        }
    };

    class material {
    public:
        inline material_var* find_var(const std::string& n, bool* f, bool c = true) {
            return hack::virtual_func<material_var*(__thiscall*)(void*, const char*, bool*, bool)>(this, 11)
                    (this, n.c_str(), f, c);
        }

        inline void release() {
            hack::virtual_func<void(__thiscall*)(void*)>(this, 13)(this);
        }

        inline void alpha_modulate(float a) {
            hack::virtual_func<void(__thiscall*)(void*, float)>(this, 27)(this, a);
        }

        inline void color_modulate(float r, float g, float b) {
            hack::virtual_func<void(__thiscall*)(void*, float, float, float)>(this, 28)(this, r, g, b);
        }
    };

    class c_material_system {
    public:
        inline material* create_material(const std::string& name, key_values* kv) {
            return hack::virtual_func<material*(__thiscall*)(void*, const char*, key_values*)>(this, 72)
                    (this, name.c_str(), kv);
        }
    };

    inline c_material_system* material_system = nullptr;
}

#endif //EV0LVE_TF_MATERIALSYSTEM_H
