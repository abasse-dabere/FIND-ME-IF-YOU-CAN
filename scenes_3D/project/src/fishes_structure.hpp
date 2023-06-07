#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "interpolation.hpp"

struct fishes_structure
{
    cgp::mesh_drawable fish_md;

    std::vector<cgp::vec3> key_positions;
    std::vector<float> key_times;

    long curr_idx = 0;
    cgp::vec3 pos;
    cgp::vec3 der;

    void initialize();
    void display(environment_structure &environment, float time);
};