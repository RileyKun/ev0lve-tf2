//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_CLIENT_MODE_H
#define EV0LVE_TF_CLIENT_MODE_H

namespace sdk {
    class c_client_mode {
    public:
        inline wchar_t* get_server_name() {
            return hack::virtual_func<wchar_t*(__thiscall*)(void*)>(this, 35)(this);
        }

        inline wchar_t* get_map_name() {
            return hack::virtual_func<wchar_t*(__thiscall*)(void*)>(this, 37)(this);
        }
    };

    inline c_client_mode* client_mode = nullptr;
}

#endif //EV0LVE_TF_CLIENT_MODE_H
