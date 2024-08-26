//
// Created by panzerfaust on 5/30/2020.
//

#ifndef EV0LVE_TF_GAME_EVENT_H
#define EV0LVE_TF_GAME_EVENT_H

namespace sdk {
    class game_event {
    public:
        inline std::string get_name() {
            return hack::virtual_func<const char*(__thiscall*)(void*)>(this, 1)(this);
        }

        inline bool get_bool(const std::string& key, bool def = false) {
            return hack::virtual_func<bool(__thiscall*)(void*, const char*, bool)>(this, 5)
                    (this, key.c_str(), def);
        }

        inline int get_int(const std::string& key, int def = 0) {
            return hack::virtual_func<int(__thiscall*)(void*, const char*, int)>(this, 6)
                    (this, key.c_str(), def);
        }

        inline float get_float(const std::string& key, float def = 0.f) {
            return hack::virtual_func<float(__thiscall*)(void*, const char*, float)>(this, 7)
                    (this, key.c_str(), def);
        }

        inline std::string get_string(const std::string& key, const std::string& def = "") {
            return hack::virtual_func<const char*(__thiscall*)(void*, const char*, const char*)>(this, 8)
                    (this, key.c_str(), def.c_str());
        }
    };

    class game_event_listener {
    public:
        virtual ~game_event_listener() {};
        virtual void fire_game_event(game_event* event) = 0;
        virtual int	get_event_debug_id() { return 42; }
    };

    class c_game_event_manager {
    public:
        inline bool add_listener(game_event_listener* l, const std::string& name, bool s = false) {
            return hack::virtual_func<bool(__thiscall*)(void*, game_event_listener*, const char*, bool)>(this, 3)
                    (this, l, name.c_str(), s);
        }

        inline void remove_listener(game_event_listener* l) {
            hack::virtual_func<void(__thiscall*)(void*, game_event_listener*)>(this, 5)
                    (this, l);
        }
    };

    inline c_game_event_manager* event_manager = nullptr;
}

#endif //EV0LVE_TF_GAME_EVENT_H
