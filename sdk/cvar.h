//
// Created by panzerfaust on 5/28/2020.
//

#ifndef EV0LVE_TF_CVAR_H
#define EV0LVE_TF_CVAR_H

namespace sdk {
    class con_command_base {
    public:
        virtual	            ~con_command_base() = 0;
        virtual	bool        is_command() = 0;
        virtual bool        is_flag_set(int) = 0;
        virtual void        add_flags(int) = 0;
        virtual void        remove_flags(int) = 0;
        virtual int	        get_flags() = 0;
        virtual const char  *get_name() = 0;
        virtual const char  *get_help_text() = 0;
        virtual bool        is_registered() = 0;
        virtual int	        get_dll_identifier() = 0;
        virtual void        create(const char*, const char*, int) = 0;
        virtual void        init() = 0;

        con_command_base            *next{};
        bool                        registered{};
        const char                  *name{};
        const char                  *help_string{};
        int	                        flags{};
    };

    class i_con_var {
    public:
        virtual void        set_value(const char*) = 0;
        virtual void        set_value(float) = 0;
        virtual void        set_value(int) = 0;
        virtual const char  *get_name() = 0;
        virtual bool        is_flag_set(int) = 0;
    };

    class con_var_value
    {
    public:
        char    *string;
        int     string_length;
        float   f_value;
        int	    n_value;
    };

    class con_var : public con_command_base, public i_con_var {
    public:
        virtual				~con_var() = 0;
        virtual bool		is_flag_set(int) = 0;
        virtual const char*	get_help_text() = 0;
        virtual bool		is_registered() = 0;
        virtual const char	*get_name() = 0;
        virtual const char	*get_base_name() = 0;
        virtual int			get_split_screen_player_slot() = 0;
        virtual void		add_flags(int) = 0;
        virtual int			get_flags() = 0;
        virtual	bool		is_command() = 0;
        virtual void		set_value(const char*) = 0;
        virtual void		set_value(float) = 0;
        virtual void		set_value(int) = 0;
        virtual void		set_value(color) = 0;
        virtual void		internal_set_value(const char*) = 0;
        virtual void		internal_set_float_value(float) = 0;
        virtual void		internal_set_int_value(int) = 0;
        virtual void		internal_set_color_value(color) = 0;
        virtual bool		clamp_value(float&) = 0;
        virtual void		change_string_value(const char*, float) = 0;
        virtual void		create(const char*, const char*, int, const char*, bool, float, bool, float, void*) = 0;
        virtual void		init() = 0;

        [[nodiscard]] inline float get_float() const { return parent->value.f_value; }
        [[nodiscard]] inline int get_int() const { return parent->value.n_value; }
        [[nodiscard]] inline color get_color() const { auto c = (uint8_t*)&parent->value.n_value; return color(c[0], c[1], c[2], c[3]); }
        [[nodiscard]] inline bool get_bool() const { return get_int() != 0; }
        [[nodiscard]] inline std::string get_string() const { auto s = parent->value.string; return s ? s : ""; }

        con_var		    *parent;
        const char	    *default_value;
        con_var_value   value;
        bool		    has_min;
        float		    min;
        bool		    has_max;
        float		    max;
        utl_vector      change_callbacks;
    };

    class c_cvar {
    public:
        inline con_var* find(const std::string& name) {
            return hack::virtual_func<con_var*(__thiscall*)(void*, const char*)>(this, hack::APP_SYSTEM_M + 7)
                    (this, name.c_str());
        }

        inline void console_color_print(const color& c, const std::string& t) {
            return hack::virtual_func<void(__thiscall*)(void*, const color&, const char*, char*)>(this, hack::APP_SYSTEM_M + 16)
                    (this, c, t.c_str(), nullptr);
        }

        inline void console_print(const std::string& t) {
            return hack::virtual_func<void(__thiscall*)(void*, const char*, char*)>(this, hack::APP_SYSTEM_M + 17)
                    (this, t.c_str(), nullptr);
        }
    };

    inline c_cvar* cvar = nullptr;
}

#endif //EV0LVE_TF_CVAR_H
