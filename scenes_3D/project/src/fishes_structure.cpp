#include "fishes_structure.hpp"

using namespace cgp;
void create_bird(cgp::mesh_drawable &fish_md);

void fishes_structure::initialize()
{
    create_bird(fish_md);
    float h = -20;
    key_positions = {{-1, 1, h}, {2, 2, h}, {-1, 1, h}, {2, 2, h}, {1.5, -1, h}, {1.5, -1, h}, {-1, -0.5, h}};
    key_times = {-2.0f, -1.0f, 0.0f, 3.0f, 4.5f, 6.0f, 8.0f};
}

void fishes_structure::display(environment_structure &environment, float time)
{
    interpolation(curr_idx, time, key_positions, key_times, pos, der);
    if (std::abs(der.x) < 100 && std::abs(der.x) > 0.001 && std::abs(der.y) < 100 && std::abs(der.y) > 0.001)
        fish_md.model.rotation = rotation_transform::from_vector_transform({-1, 0, 0}, cgp::normalize(cgp::vec3{der.x, der.y, 0}));
    fish_md.model.translation = pos;
    if (pos.z < -5)
        draw(fish_md, environment);
}

void create_bird(cgp::mesh_drawable &fish_md)
{
    fish_md.initialize_data_on_gpu(mesh_load_file_obj("assets/blender_obj/Fishes/fish_0.obj"));
    fish_md.texture.load_and_initialize_texture_2d_on_gpu("assets/blender_obj/Fishes/fish_0_texture.jpg");
}