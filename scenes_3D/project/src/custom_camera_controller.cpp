#include "custom_camera_controller.hpp"

namespace cgp
{
    void custom_camera_controller::idle_frame(mat4 &camera_matrix_view)
    {
        // Preconditions
        assert_cgp_no_msg(inputs != nullptr);
        assert_cgp_no_msg(window != nullptr);
        if (!is_active)
            return;

        float magnitude = 75.0f * inputs->time_interval;
        float turn_mag = 1.0f;

        vec3 const &up = camera_model.axis_of_rotation;
        vec3 const &front = camera_model.front();
        vec3 const &right = camera_model.right();

        // run mode
        if (inputs->keyboard.shift)
        {
            magnitude = 5.0f * magnitude;
            turn_mag = 5.0f * turn_mag;
        }

        // Constraint displacement on the plane
        vec3 move_front = front - dot(front, up) * up;
        vec3 move_right = right - dot(right, up) * up;

        if (inputs->keyboard.up || inputs->keyboard.is_pressed(GLFW_KEY_W))
            if (cgp::norm(camera_model.position_camera + magnitude * move_front) < 355)
                camera_model.position_camera += magnitude * move_front;
        if (inputs->keyboard.down || inputs->keyboard.is_pressed(GLFW_KEY_S))
            if (cgp::norm(camera_model.position_camera - magnitude * move_front) < 355)
                camera_model.position_camera -= magnitude * move_front;
        if (inputs->keyboard.left || inputs->keyboard.is_pressed(GLFW_KEY_A))
            camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, turn_mag * 0.01);
        // camera_model.position_camera -= 0.5f * magnitude * move_right;
        if (inputs->keyboard.right || inputs->keyboard.is_pressed(GLFW_KEY_D))
            camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -turn_mag * 0.01);
        // camera_model.position_camera += 0.5f * magnitude * move_right;

        update(camera_matrix_view);
    }
}