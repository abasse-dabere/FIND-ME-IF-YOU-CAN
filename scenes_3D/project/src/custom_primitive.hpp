#pragma once

#include "cgp/cgp.hpp"

cgp::mesh create_demi_sphere_txt(long txt = 1, float radius = 1.0f, cgp::vec3 center = {0, 0, 0}, int Nu = 40, int Nv = 20);
cgp::mesh mesh_primitive_grid_txt(cgp::vec3 const &p00 = {0, 0, 0}, cgp::vec3 const &p10 = {1, 0, 0}, cgp::vec3 const &p11 = {1, 1, 0}, cgp::vec3 const &p01 = {0, 1, 0}, int Nu = 10, int Nv = 10, long txt = 10);