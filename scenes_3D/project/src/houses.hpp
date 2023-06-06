#pragma once

#include "cgp/cgp.hpp"
cgp::hierarchy_mesh_drawable create_house_model_0(float height,
                                                  cgp::vec3 &center,
                                                  cgp::vec3 &painting_position,
                                                  float &painting_min_dist,
                                                  cgp::opengl_shader_structure &shader_illumination);