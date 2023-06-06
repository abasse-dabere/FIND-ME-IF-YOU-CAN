#pragma once
#include <random>

#include "cgp/cgp.hpp"

// Compute the interpolated position p(t) given a time t and the set of key_positions and key_frame
void interpolation(long &curr_idx, float t, std::vector<cgp::vec3> &key_positions, std::vector<float> &key_times, cgp::vec3 &pos, cgp::vec3 &der);