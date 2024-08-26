//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_INPUT_H
#define EV0LVE_TF_INPUT_H

namespace sdk {
    class joystick_axis {
    public:
        unsigned int axis_flags{};
        unsigned int axis_map{};
        unsigned int control_map{};
    };

    class camera_third_person_data {
    public:
        float	pitch{};
        float	yaw{};
        float	dist{};
        float	lag{};
        vec3	hull_min{};
        vec3	hull_max{};
    };

    class c_input {
    public:
        virtual void init_all() = 0;
        virtual void shutdown_all() = 0;
        virtual int	get_button_bits(int) = 0;
        virtual void create_move(int, float, bool) = 0;
        virtual void extra_mouse_sample(float, bool) = 0;
        virtual bool write_user_cmd(void*, int, int, bool) = 0;

        inline int is_third_person() {
            return hack::virtual_func<int(*)(void*)>(this, 31)(this);
        }

        inline void to_third_person() {
            hack::virtual_func<void(*)(void*)>(this, 32)(this);
        }

        inline void to_first_person() {
            hack::virtual_func<void(*)(void*)>(this, 33)(this);
        }

        inline user_cmd* get_user_cmd(int seq) {
            return &commands[seq % 90];
        }

        inline verified_cmd* get_verified_cmd(int seq) {
            return &verified_commands[seq % 90];
        }

        inline void erase(int seq) {
            *reinterpret_cast<uint32_t*>(commands + (seq % 90)) = 0;
            *reinterpret_cast<uint32_t*>(verified_commands + (seq % 90)) = 0;
        }

        bool is_mouse_initialized{}; //0x0004
        bool is_mouse_active{}; //0x0005
        bool is_joystick_initialized{}; //0x0006
        bool has_joysticks{}; //0x0007
        float accum_mouse_x{}; //0x0008
        float accum_mouse_y{}; //0x000C
        float prev_mouse_x{}; //0x0010
        float prev_mouse_y{}; //0x0014
        float joystick_sample_time{}; //0x0018
        float keyboard_sample_time{}; //0x001C
        bool should_restore_spi{}; //0x0020

    private:
        char pad_0021[116]{}; //0x0021

    public:
        bool is_camera_in_third_person{}; //0x0095

    private:
        char pad_0096[22]{}; //0x0096

    public:
        ang previous_view_angles; //0x00AC
        float last_forward_move{}; //0x00B8

    private:
        char pad_00BC[64]{}; //0x00BC

    public:
        user_cmd *commands{}; //0x00FC
        verified_cmd *verified_commands{}; //0x0100
        camera_third_person_data *third_person_data{}; //0x0104
        uint32_t selected_weapon{}; //0x0108
    };

    inline c_input* input = nullptr;
}

#endif //EV0LVE_TF_INPUT_H
