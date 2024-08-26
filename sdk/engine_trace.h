//
// Created by panzerfaust on 6/5/2020.
//

#ifndef EV0LVE_TF_ENGINE_TRACE_H
#define EV0LVE_TF_ENGINE_TRACE_H

namespace sdk {
    class ray {
    public:
        vec_aligned start;
        vec_aligned delta;
        vec_aligned offset;
        vec_aligned extents;
        bool is_ray{};
        bool is_swept{};

        inline void init(const vec3& s, const vec3& e) {
            delta = e - s;
            start = s;

            is_swept = (delta.len_sqr() != 0.f);
            is_ray = true;
        }

        inline void init(const vec3& s, const vec3& e, const vec3& mins, const vec3& maxs) {
            delta = e - s;
            extents = maxs - mins; extents *= 0.5f;
            offset = mins + maxs; offset *= 0.5f;
            start = offset + s; offset *= -1.f;

            is_swept = (delta.len_sqr() != 0.f);
            is_ray = (extents.len_sqr() < 1e-6);
        }
    };

    class trace_plane {
    public:
        vec3 normal;
        float dist;
        char type;
        char sign_bits;
        char pad[2];
    };

    class base_trace {
    public:
        vec3 start;
        vec3 end;
        trace_plane plane;
        float fraction;
        int contents;
        uint16_t flags;
        bool all_solid;
        bool start_solid;
    };

    class trace_surface {
    public:
        const char* name;
        short surface_props;
        uint16_t flags;
    };

    class trace : public base_trace {
    public:
        float fraction_left_solid;
        trace_surface surface;
        int hitgroup;
        short physics_bone;
        entity* ent;
        int hitbox;

        inline bool did_hit() const {
            return fraction < 1.f || all_solid || start_solid;
        }
    };

    enum trace_type {
        trace_everything = 0,
        trace_world_only,
        trace_entities_only,
        trace_everything_filter_props,
    };

    class trace_filter_base {
    public:
        virtual bool should_hit_entity(entity* ent, int contents_mask) = 0;
        virtual trace_type get_trace_type() const = 0;
    };

    class trace_filter : public trace_filter_base {
    public:
        virtual trace_type get_trace_type() const {
            return trace_everything;
        }

        virtual bool should_hit_entity(entity* ent, int contents_mask) {
            if (skip && ent)
                return skip->index() != ent->index();
            return true;
        }

        entity* skip{};
    };

    class trace_filter_entities_only : public trace_filter_base {
    public:
        virtual trace_type get_trace_type() const {
            return trace_entities_only;
        }
    };

    class trace_filter_world_only : public trace_filter_base {
    public:
        virtual bool should_hit_entity(entity* ent, int contents_mask) {
            return false;
        }

        virtual trace_type get_trace_type() const {
            return trace_world_only;
        }
    };

    enum contents {
        contents_empty                  = 0,
        contents_solid		            = 0x1,
        contents_window		            = 0x2,
        contents_aux		            = 0x4,
        contents_grate		            = 0x8,
        contents_slime		            = 0x10,
        contents_water		            = 0x20,
        contents_mist		            = 0x40,
        contents_opaque		            = 0x80,
        contents_testfogvolume	        = 0x100,
        contents_unused5		        = 0x200,
        contents_unused6		        = 0x4000,
        contents_team1			        = 0x800,
        contents_team2			        = 0x1000,
        contents_ignore_nodraw_opaque   = 0x2000,
        contents_moveable		        = 0x4000,
        contents_areaportal		        = 0x8000,
        contents_playerclip		        = 0x10000,
        contents_monsterclip	        = 0x20000,
        contents_current_0		        = 0x40000,
        contents_current_90		        = 0x80000,
        contents_current_180	        = 0x100000,
        contents_current_270	        = 0x200000,
        contents_current_up		        = 0x400000,
        contents_current_down	        = 0x800000,
        contents_origin			        = 0x1000000,
        contents_monster		        = 0x2000000,
        contents_debris			        = 0x4000000,
        contents_detail			        = 0x8000000,
        contents_translucent	        = 0x10000000,
        contents_ladder			        = 0x20000000,
        contents_hitbox			        = 0x40000000,
    };

    enum mask {
        mask_all					= 0xffffffff,
        mask_solid					= contents_solid|contents_moveable|contents_window|contents_monster|contents_grate,
        mask_playersolid			= contents_solid|contents_moveable|contents_playerclip|contents_window|contents_monster|contents_grate,
        mask_npcsolid				= contents_solid|contents_moveable|contents_monsterclip|contents_window|contents_monster|contents_grate,
        mask_water					= contents_water|contents_moveable|contents_slime,
        mask_opaque					= contents_solid|contents_moveable|contents_opaque,
        mask_opaque_and_npcs		= mask_opaque|contents_monster,
        mask_visible				= mask_opaque|contents_ignore_nodraw_opaque,
        mask_visible_and_npcs		= mask_opaque_and_npcs|contents_ignore_nodraw_opaque,
        mask_shot					= contents_solid|contents_moveable|contents_monster|contents_window|contents_debris|contents_hitbox,
        mask_shot_hull				= contents_solid|contents_moveable|contents_monster|contents_window|contents_debris|contents_grate,
        mask_shot_portal			= contents_solid|contents_moveable|contents_window,
        mask_shot_brushonly			= contents_solid|contents_moveable|contents_window|contents_debris,
        mask_solid_brushonly		= contents_solid|contents_moveable|contents_window|contents_grate,
        mask_playersolid_brushonly	= contents_solid|contents_moveable|contents_window|contents_playerclip|contents_grate,
        mask_npcsolid_brushonly		= contents_solid|contents_moveable|contents_window|contents_monsterclip|contents_grate,
        mask_npcworldstatic			= contents_solid|contents_window|contents_monsterclip|contents_grate,
        mask_splitareaportal		= contents_water|contents_slime,
    };

    class c_engine_trace {
    public:
        inline int get_point_contents(const vec3& pos, entity** ent) {
            return hack::virtual_func<int(__thiscall*)(void*, const vec3&, entity**)>(this, 0)
                    (this, pos, ent);
        }

        inline void trace_ray(const ray& r, uint32_t mask, trace_filter_base* filter, trace* t) {
            hack::virtual_func<void(__thiscall*)(void*, const ray&, uint32_t, trace_filter_base*, trace*)>(this, 4)
                    (this, r, mask, filter, t);
        }
    };

    inline c_engine_trace* engine_trace = nullptr;
}

#endif //EV0LVE_TF_ENGINE_TRACE_H
