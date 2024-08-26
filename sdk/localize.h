//
// Created by panzerfaust on 5/30/2020.
//

#ifndef EV0LVE_TF_LOCALIZE_H
#define EV0LVE_TF_LOCALIZE_H

namespace sdk {
    class c_localize {
    public:
        inline std::string find(const std::string& key) {
            auto r = hack::virtual_func<const char*(__thiscall*)(void*, const char*)>(this, 15)
                    (this, key.c_str());

            if (key == r) {
                if (!strcmp(r, "#TF_Weapon_Builder"))
                    return "Builder";
            }

            return r ? r : "";
        }
    };

    inline c_localize* localize = nullptr;
}

#endif //EV0LVE_TF_LOCALIZE_H
