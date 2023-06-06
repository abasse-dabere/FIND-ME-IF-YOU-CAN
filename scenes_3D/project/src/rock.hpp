#pragma once

#include "cgp/cgp.hpp"

cgp::mesh_drawable create_rock_model_0(float &a,
                                       float &b,
                                       float &h,
                                       cgp::vec3 const &center,
                                       cgp::opengl_shader_structure const &shader_illumination);
cgp::mesh create_rock_model_1(float r,
                              float h);
