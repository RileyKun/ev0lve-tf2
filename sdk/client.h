//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_CLIENT_H
#define EV0LVE_TF_CLIENT_H

namespace sdk {
    enum frame_stage {
        fs_undefined = -1,
        fs_start,
        fs_net_update_start,
        fs_net_post_data_update_start,
        fs_net_post_data_update_end,
        fs_net_update_end,
        fs_render_start,
        fs_render_end
    };

    class c_client {
    public:
        inline client_class* get_all_classes() {
            return hack::virtual_func<client_class*(__thiscall*)(void*)>(this, 8)(this);
        }

        inline bool write_usercmd(bf_write* buf, int from, int to, bool is_new) {
            return hack::virtual_func<bool(__thiscall*)(void*, bf_write*, int, int, bool)>(this, 23)
                    (this, buf, from, to, is_new);
        }
    };

    inline c_client* client = nullptr;
}

#endif //EV0LVE_TF_CLIENT_H
