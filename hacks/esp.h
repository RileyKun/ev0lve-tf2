//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_ESP_H
#define EV0LVE_TF_ESP_H

#include <ev0lve.h>
#include <gui/gui.h>
#include <sdk/sdk.h>
#include <types/types.h>

namespace hack {
    class c_esp {
    public:
        void work();
        void draw_prepare();
        void draw();

        inline void reset() {
            if (!players.empty())
                players.clear();
        }

        class player {
        public:
            sdk::entity* ent{};
            sdk::vec3 pos;
            sdk::vec3 world_maxs;

            std::string name;
            bool is_dormant = false;
            bool is_cloaked = false;
            bool is_aiming = false;
            bool is_ubercharged = false;
            bool is_overhealed = false;
            char life_state = 0;
            int health = 0;
            int max_health = 0;
            int class_id = 0;
            float distance = 0.f;

            bool has_weapon = false;
            bool is_primary = false;
            std::string wep_name;
            int clip = 0;
            int fired = 0;
            int max_clip = 0;

            _VEC2 mins;
            _VEC2 maxs;

            bool is_offscreen = false;
            float dormancy_time = 0.f;

            bool did_init = false;
            gui::anim_val alpha;
            gui::anim_val hp;
            gui::anim_val ammo;
        };

        class bottom_info {
        public:
            bottom_info(const std::string& s) {
                text = s;
            }
            bottom_info(float min, float max, float v, const std::string& s = "") {
                is_bar = true;

                this->min = min;
                this->max = max;
                this->v = v;
                this->text = s;
            }

            bool is_bar = false;

            std::string text;
            float min = 0.f;
            float max = 0.f;
            float v = 0.f;
        };

        class side_info {
        public:
            side_info(const std::string& s, const _COLOR& c) {
                text = s;
                col = c;
            }

            std::string text;
            _COLOR col;
        };

        std::unordered_map<uint32_t, player> players;
    private:
        inline void draw_box(const player& p);
        inline void draw_health(const player& p);
        inline void draw_name(const player& p);
        inline void draw_bottom(const player& p);
        inline void draw_side(const player& p);
        inline void draw_arrow(const player& p);

        std::vector<uint32_t> to_erase;
    };

    inline c_esp esp{};
}

#endif //EV0LVE_TF_ESP_H
