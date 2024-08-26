//
// Created by panzerfaust on 6/6/2020.
//

#ifndef EV0LVE_TF_MODELINFO_H
#define EV0LVE_TF_MODELINFO_H

namespace sdk {
    class c_model_info {
    public:
        inline const model *get_model(int i) {
            return hack::virtual_func<const model *(__thiscall *)(void *, int)>(this, 1)
                    (this, i);
        }

        inline int get_model_index(const std::string& name) {
            return hack::virtual_func<int(__thiscall*)(void*, const char*)>(this, 2)
                    (this, name.c_str());
        }

        inline std::string get_model_name(const model* m) {
            return hack::virtual_func<const char*(__thiscall*)(void*, const model*)>(this, 3)
                    (this, m);
        }

        inline studio_hdr* get_studio_model(const model* m) {
            return hack::virtual_func<studio_hdr*(__thiscall*)(void*, const model*)>(this, 28)
                    (this, m);
        }
    };

    inline c_model_info* model_info = nullptr;
}

#endif //EV0LVE_TF_MODELINFO_H
