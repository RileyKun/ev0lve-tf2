//
// Created by panzerfaust on 6/1/2020.
//

#ifndef EV0LVE_TF_KEYVALUES_H
#define EV0LVE_TF_KEYVALUES_H

namespace sdk {
    class key_values {
    public:
        inline key_values() {
            key_name = -1;
            data_type = 0;

            sub = nullptr;
            peer = nullptr;
            chain = nullptr;

            s_value = nullptr;
            w_value = nullptr;
            p_value = nullptr;

            has_escape_sequences = false;
            eval_conditionals = true;

            memset(unused, 0, sizeof(unused));
        }

        void set_name(const std::string& name);

        void set_string(const std::string& key, const std::string& value);
        void set_int(const std::string& key, int value);
        void set_float(const std::string& key, float value);
        void set_color(const std::string& key, color value);

        inline void set_bool(const std::string& key, bool value) {
            set_int(key, value ? 1 : 0);
        }

        inline int get_key_name() {
            return key_name;
        }

    private:
        int key_name;
        char* s_value;
        wchar_t* w_value;

        union {
            int n_value;
            float f_value;
            void* p_value;
            unsigned char c_value[4];
        };

        char data_type;
        char has_escape_sequences;
        char eval_conditionals;
        char unused[1];

        key_values* peer;
        key_values* sub;
        key_values* chain;
    };
}

#endif //EV0LVE_TF_KEYVALUES_H
