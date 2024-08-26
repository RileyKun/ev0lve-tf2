//
// Created by panzerfaust on 6/8/2020.
//

#ifndef EV0LVE_TF_GAME_MOVEMENT_H
#define EV0LVE_TF_GAME_MOVEMENT_H

namespace sdk {
    class move_data {
    public:
        bool	first_run_of_functions : 1;
        bool	game_code_moved_player : 1;
        handle	player_handle;
        int		impulse_command;
        ang	    view_angles;
        ang	    abs_view_angles;
        int		buttons;
        int		old_buttons;
        float	forward_move;
        float	old_forward_move;
        float	side_move;
        float	up_move;
        float	max_speed;
        float	client_max_speed;
        vec3	velocity;
        ang		angles;
        ang		old_angles;
        float	out_step_height;
        vec3	out_wish_vel;
        vec3	out_jump_vel;
        vec3	constraint_center;
        float	constraint_radius;
        float	constraint_width;
        float	constraint_speed_factor;
        vec3	abs_origin;
    };

    class c_game_movement {
    public:
        inline void process_movement(entity* p, move_data* d) {
            hack::virtual_func<void(__thiscall*)(void*, entity*, move_data*)>(this, 1)
                    (this, p, d);
        }

        inline void start_track_prediction_errors(entity* p) {
            hack::virtual_func<void(__thiscall*)(void*, entity*)>(this, 2)(this, p);
        }

        inline void finish_track_prediction_errors(entity* p) {
            hack::virtual_func<void(__thiscall*)(void*, entity*)>(this, 3)(this, p);
        }
    };

    class c_move_helper {
    public:

    };

    inline c_game_movement* game_movement = nullptr;
    inline c_move_helper* move_helper = nullptr;
}

#endif //EV0LVE_TF_GAME_MOVEMENT_H
