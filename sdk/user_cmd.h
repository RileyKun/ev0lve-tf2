//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_USER_CMD_H
#define EV0LVE_TF_USER_CMD_H

#include <misc/crc32.h>

namespace sdk {
    enum button : uint32_t {
        in_attack      = 1U << 0U,
        in_jump        = 1U << 1U,
        in_duck        = 1U << 2U,
        in_forward     = 1U << 3U,
        in_back        = 1U << 4U,
        in_use         = 1U << 5U,
        in_cancel      = 1U << 6U,
        in_left        = 1U << 7U,
        in_right       = 1U << 8U,
        in_move_left   = 1U << 9U,
        in_move_right  = 1U << 10U,
        in_attack2     = 1U << 11U,
        in_run         = 1U << 12U,
        in_reload      = 1U << 13U,
        in_alt1        = 1U << 14U,
        in_alt2        = 1U << 15U,
        in_score       = 1U << 16U,
        in_speed       = 1U << 17U,
        in_walk        = 1U << 18U,
        in_zoom        = 1U << 19U,
        in_weapon1     = 1U << 20U,
        in_weapon2     = 1U << 21U,
        in_bull_rush   = 1U << 22U,
        in_grenade1    = 1U << 23U,
        in_grenade2    = 1U << 24U,
        in_look_spin   = 1U << 25U,
    };

    class user_cmd {
    public:
        inline uint32_t get_checksum() {
            uint32_t crc;

            crc32::init(&crc);
            crc32::process_buffer(&crc, &command_number, sizeof(command_number));
            crc32::process_buffer(&crc, &tick_count, sizeof(tick_count));
            crc32::process_buffer(&crc, &view_angles, sizeof(view_angles));
            crc32::process_buffer(&crc, &forward_move, sizeof(forward_move));
            crc32::process_buffer(&crc, &side_move, sizeof(side_move));
            crc32::process_buffer(&crc, &up_move, sizeof(up_move));
            crc32::process_buffer(&crc, &buttons, sizeof(buttons));
            crc32::process_buffer(&crc, &impulse, sizeof(impulse));
            crc32::process_buffer(&crc, &weapon_select, sizeof(weapon_select));
            crc32::process_buffer(&crc, &weapon_sub_type, sizeof(weapon_sub_type));
            crc32::process_buffer(&crc, &random_seed, sizeof(random_seed));
            crc32::process_buffer(&crc, &mouse_dx, sizeof(mouse_dx));
            crc32::process_buffer(&crc, &mouse_dy, sizeof(mouse_dy));
            crc32::final(&crc);

            return crc;
        }

        void fix_movement(ang& wish);

        void* members{};
        int command_number{};
        int tick_count{};
        ang view_angles;
        float forward_move{};
        float side_move{};
        float up_move{};
        uint32_t buttons{};
        uint8_t impulse{};
        int weapon_select{};
        int weapon_sub_type{};
        int random_seed{};
        short mouse_dx{};
        short mouse_dy{};
        bool has_been_predicted{};
    };

    class verified_cmd {
    public:
        user_cmd cmd;
        uint32_t crc{};
    };
}

#endif //EV0LVE_TF_USER_CMD_H
