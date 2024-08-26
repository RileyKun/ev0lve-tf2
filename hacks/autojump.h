//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_AUTOJUMP_H
#define EV0LVE_TF_AUTOJUMP_H

namespace hack {
    class c_autojump {
    public:
        void work();

    private:
        void auto_jump();
        void auto_strafe() const;

        bool is_hopping = false;
    };

    inline c_autojump autojump{};
}

#endif //EV0LVE_TF_AUTOJUMP_H
