//
// Created by panzerfaust on 6/9/2020.
//

#ifndef EV0LVE_TF_GAME_PREDICTION_H
#define EV0LVE_TF_GAME_PREDICTION_H

namespace sdk {
    class c_game_prediction {
    public:
        inline void set_view_angles(ang& a) {
            hack::virtual_func<void(__thiscall*)(void*, ang&)>(this, 11)
                    (this, a);
        }

        inline void setup_move(entity* p, user_cmd* c, c_move_helper* h, move_data* d) {
            hack::virtual_func<void(__thiscall*)(void*, entity*, user_cmd*, c_move_helper*, move_data*)>(this, 18)
                    (this, p, c, h, d);
        }

        inline void finish_move(entity* p, user_cmd* c, move_data* d) {
            hack::virtual_func<void(__thiscall*)(void*, entity*, user_cmd*, move_data*)>(this, 19)
                    (this, p, c, d);
        }
    };

    inline c_game_prediction* game_prediction = nullptr;
}

#endif //EV0LVE_TF_GAME_PREDICTION_H
