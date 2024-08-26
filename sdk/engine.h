//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_ENGINE_H
#define EV0LVE_TF_ENGINE_H

namespace sdk {
    class player_info {
    public:
        char        name[32];
        int         user_id;
        char        guid[33];
        uint32_t    friends_id;
        char        friends_name[32];
        bool        is_bot;
        bool        is_tv;
        uint32_t    files[4];
        uint8_t     downloaded;

        inline std::string get_name() { return name; }
        inline std::string get_guid() { return guid; }
        inline std::string get_friends_name() { return friends_name; }
    };

    class c_engine {
    public:
        inline bool get_player_info(int id, player_info* info) {
            return hack::virtual_func<bool(__thiscall*)(void*, int, player_info*)>(this, 8)(this, id, info);
        }

        inline int user_id_to_ent_num(int user_id) {
            return hack::virtual_func<int(__thiscall*)(void*, int)>(this, 9)(this, user_id);
        }

        inline int get_local_player() {
            return hack::virtual_func<int(__thiscall*)(void*)>(this, 12)(this);
        }

        inline void get_view_angles(ang& v) {
            return hack::virtual_func<void(__thiscall*)(void*, ang&)>(this, 19)(this, v);
        }

        inline void set_view_angles(ang& v) {
            return hack::virtual_func<void(__thiscall*)(void*, ang&)>(this, 20)(this, v);
        }

        inline bool is_in_game() {
            return hack::virtual_func<bool(__thiscall*)(void*)>(this, 26)(this);
        }

        inline bool is_connected() {
            return hack::virtual_func<bool(__thiscall*)(void*)>(this, 27)(this);
        }

        inline net_channel_info* get_net_channel_info() {
            return hack::virtual_func<net_channel_info*(__thiscall*)(void*)>(this, 72)(this);
        }
    };

    inline c_engine* engine = nullptr;
}

#endif //EV0LVE_TF_ENGINE_H
