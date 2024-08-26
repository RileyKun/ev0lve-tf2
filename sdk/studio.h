//
// Created by panzerfaust on 6/2/2020.
//

#ifndef EV0LVE_TF_STUDIO_H
#define EV0LVE_TF_STUDIO_H

namespace sdk {
    class studio_hitbox {
    public:
        int bone{};
        int group{};
        vec3 bb_min;
        vec3 bb_max;
        int hitbox_name_index{};
        int unused[8]{};

        inline const char* name() {
            if (!hitbox_name_index)
                return "";

            return reinterpret_cast<const char*>(this) + hitbox_name_index;
        }
    };

    class studio_hitbox_set {
    public:
        int name_index;
        int num_hitboxes;
        int hitbox_index;

        inline studio_hitbox* hitbox(int i) {
            return (studio_hitbox*)((unsigned char*)this + hitbox_index) + i;
        }
    };

    class studio_hdr {
    public:
        int id{};
        int version{};
        int checksum{};
        char name[64]{};
        int length{};
        vec3 eye_position;
        vec3 illum_position;
        vec3 hull_min;
        vec3 hull_max;
        vec3 view_bb_min;
        vec3 view_bb_max;
        int flags{};
        int num_bones{};
        int bone_index{};
        int num_bone_controllers{};
        int bone_controller_index{};
        int num_hitbox_sets{};
        int hitbox_set_index{};

        inline studio_hitbox_set* hitbox_set(int i) {
            return (studio_hitbox_set*)((unsigned char*)this + hitbox_set_index) + i;
        }
    };

    class studio_hw_data {
    public:
        int root_lod;
        int num_lods;
        void* lods;
        int num_studio_meshes;
    };
}

#endif //EV0LVE_TF_STUDIO_H
