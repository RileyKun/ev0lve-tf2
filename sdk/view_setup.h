//
// Created by panzerfaust on 6/4/2020.
//

#ifndef EV0LVE_TF_VIEW_SETUP_H
#define EV0LVE_TF_VIEW_SETUP_H

namespace sdk {
    class view_setup {
    public:
        int x;
        int unscaled_x;
        int y;
        int unscaled_y;
        int width;
        int unscaled_with;
        int height;
        int stereo_eye;
        int unscaled_height;
        bool ortho;
        float ortho_left;
        float ortho_top;
        float ortho_right;
        float ortho_bottom;
        float fov;
        float fov_viewmodel;
        vec3 origin;
        ang angles;
        float z_near;
        float z_far;
        float z_near_viewmodel;
        float z_far_viewmodel;
        bool larger_screen;
        float aspect_ratio;
        bool off_center;
        float off_center_top;
        float off_center_bottom;
        float off_center_left;
        float off_center_right;
        bool do_post_processing;
        bool cache_full_scene_state;
        bool view_to_projection_override;
        float view_to_projection[4][4];
    };
}

#endif //EV0LVE_TF_VIEW_SETUP_H
