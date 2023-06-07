#include "tree.hpp"
#include "custom_primitive.hpp"

using namespace cgp;

cgp::mesh create_tree_model_0(float height)
{
    height *= 4;
    cgp::mesh m;
    float ratio = 6 * height / 25.0f;
    mesh tronc = mesh_primitive_cylinder(ratio / 8.0f, cgp::vec3{0, 0, 0}, cgp::vec3{0, 0, height / 2}, 10, 20, true);
    tronc.color.fill({204 / 256.0f, 102 / 256.0f, 0});
    m.push_back(tronc);
    mesh folliage1 = mesh_primitive_cylinder(ratio, cgp::vec3{0, 0, ratio}, cgp::vec3{0, 0, ratio * (1.0f + 1 / 3.0f)}, 10, 20, true);
    folliage1.color.fill({0, 102 / 256.0f, 0});
    m.push_back(folliage1);
    mesh folliage2 = create_demi_sphere_txt(1, ratio, cgp::vec3{0, 0, 0}, 20, 10);
    folliage2.color.fill({0, 102 / 256.0f, 0});
    folliage2.apply_to_position(cgp::mat4::build_rotation_from_axis_angle(cgp::vec3{0, 1, 0}, 3.14f));
    folliage2.apply_to_position(cgp::mat4::build_scaling(cgp::vec3{1, 1, 2.5f}));
    folliage2.apply_to_position(cgp::mat4::build_translation(cgp::vec3{0, 0, height * 0.35f}));
    m.push_back(folliage2);
    return m;
}

cgp::mesh create_tree_model_1(float height)
{
    cgp::mesh m;

    cgp::mesh folliage = mesh_primitive_cone(height, 2 * height, {0, 0, height}, {0, 0, 1}, false, 10, 5);
    folliage.push_back(mesh_primitive_cone(height, 2 * height, {0, 0, 3 * height / 2}, {0, 0, 1}, false, 10, 5));
    folliage.push_back(mesh_primitive_cone(height, 2 * height, {0, 0, 2 * height}, {0, 0, 1}, false, 10, 5));
    folliage.color.fill({0, 102 / 256.0f, 0});

    cgp::mesh tronc = mesh_primitive_cylinder(1.5f * height / 10.0f, {0, 0, 0}, {0, 0, 2 * height}, 10, 20, false);
    tronc.color.fill({204 / 256.0f, 102 / 256.0f, 0});

    m.push_back(folliage);
    m.push_back(tronc);
    return m;
}

cgp::mesh create_tree_model_2(float height)
{
    cgp::mesh m;
    long N_folliage = rand() % 5 + 3;
    float h_trunk = rand_interval(height / 2, 3 * height / 2);
    float r_folliage = rand_interval(height / 2, height);
    cgp::vec3 c_folliage = {0, 0, 2 * h_trunk / 3};

    cgp::mesh folliage;
    for (size_t i = 0; i < N_folliage; i++)
    {
        folliage.push_back(mesh_primitive_cone(r_folliage, 2 * r_folliage, c_folliage, {0, 0, 1}, true, 10, 5));
        c_folliage += {0, 0, 3 * r_folliage / 4};
        r_folliage -= r_folliage / 10;
    }
    folliage.color.fill({0, 102 / 256.0f, 0});

    cgp::mesh tronc = mesh_primitive_cylinder(1.5f * height / 10.0f, {0, 0, 0}, {0, 0, h_trunk}, 10, 20, false);
    tronc.color.fill({204 / 256.0f, 102 / 256.0f, 0});

    m.push_back(folliage);
    m.push_back(tronc);

    return m;
}

cgp::mesh create_tree_model_3(float height)
{
    cgp::mesh m;
    long N_folliage = rand() % 2 + 3;
    float h_trunk = rand_interval(height, 3 * height / 2);
    float r_folliage = rand_interval(height / 2, height);
    cgp::vec3 c_folliage = {0, 0, h_trunk};

    cgp::mesh folliage;
    for (size_t i = 0; i < N_folliage; i++)
    {
        mesh cube_mesh = mesh_primitive_cube();
        cube_mesh.apply_to_position(cgp::mat4::build_translation(c_folliage) * cgp::mat4::build_scaling({1.5 * r_folliage, 1.5 * r_folliage, r_folliage / 1.5f}));
        folliage.push_back(cube_mesh);
        c_folliage += {0, 0, r_folliage};
    }
    folliage.color.fill({0, 102 / 256.0f, 0});

    cgp::mesh tronc = mesh_primitive_cylinder(1.5f * height / 10.0f, {0, 0, 0}, {0, 0, h_trunk + N_folliage * r_folliage / 1.5f}, 10, 20, false);
    tronc.color.fill({204 / 256.0f, 102 / 256.0f, 0});

    m.push_back(folliage);
    m.push_back(tronc);

    return m;
}

cgp::mesh create_rd_tree(float height, float m0, float m2, float m3)
{
    float pm0 = m0 / (m0 + m2 + m3);
    float pm2 = pm0 + m2 / (m0 + m2 + m3);
    float pr = rand_interval();
    if (pr < pm0)
        return create_tree_model_0(height);
    if (pr < pm2)
        return create_tree_model_2(height);
    return create_tree_model_3(height);
}