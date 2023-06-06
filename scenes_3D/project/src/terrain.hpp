#pragma once

#include "cgp/cgp.hpp"

struct perlin_noise_parameters
{
    float persistency = 0.5f;
    float frequency_gain = 5.0f;
    int octave = 6;
    float terrain_height = 0.5f;
};

float evaluate_terrain_height(float x, float y, float terrain_length);
cgp::mesh create_terrain_mesh(int N, float length);
std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length);
bool dont_intersect(float x, float y, float min_dist, std::vector<cgp::vec3> &vec, int fin);
