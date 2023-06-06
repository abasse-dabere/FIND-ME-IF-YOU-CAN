#pragma once
#include "cgp/cgp.hpp"

// #include "../camera_controller_first_person_euler/camera_controller_first_person_euler.hpp"

namespace cgp
{

    struct custom_camera_controller : camera_controller_2d_displacement
    {
        void idle_frame(mat4 &camera_matrix_view);
    };

}