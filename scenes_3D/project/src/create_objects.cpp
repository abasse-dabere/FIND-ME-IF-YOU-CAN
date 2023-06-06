#include "create_objects.hpp"
using namespace cgp;

void create_object_land0(cgp::hierarchy_mesh_drawable &windmill0,
                         cgp::mesh_drawable &rock0,
                         cgp::mesh_drawable &tree,
                         cgp::mesh_drawable &grass,
                         long const &N_herbe,
                         cgp::opengl_shader_structure const &shader_illumination,
                         cgp::opengl_shader_structure const &shader_grass,
                         cgp::vec3 const &center,
                         float const &scale,
                         bool &bLand0)
{
    float a = 50 * scale;
    float b = 32 * scale;
    float h = 30 * scale;

    rock0 = create_rock_model_0(a, b, h, center, shader_illumination);

    windmill0 = create_windmill(scale * 1.5, shader_illumination);
    windmill0["Base"].transform_local.translation += cgp::vec3{a / 2.0f, b / 3.0f, 0.0f} + center;

    mesh tree0_mesh = create_tree_model_0(25.0f * scale);
    tree0_mesh.apply_translation_to_position(center);

    mesh tree1_mesh = create_tree_model_1(7.0f * scale);
    tree1_mesh.apply_translation_to_position(center + cgp::vec3{-a / 2, b / 3, 0});

    tree0_mesh.push_back(tree1_mesh);
    tree.initialize_data_on_gpu(tree0_mesh);
    tree.material.phong.specular = 0.0f;
    tree.shader = shader_illumination;

    mesh herbe = mesh_load_file_obj("assets/grass/Low Grass.obj");
    herbe.apply_rotation_to_position({1, 0, 0}, 3.14f / 2.0f);
    herbe.apply_scaling_to_position(scale * 30);
    grass.initialize_data_on_gpu(herbe);
    grass.texture.load_and_initialize_texture_2d_on_gpu("assets/grass/Grass.png");
    grass.shader.load("shaders/mesh_grass/mesh_grass.vert.glsl", "shaders/mesh_grass/mesh_grass.frag.glsl");
    // grass.model.scaling = scale * 5;
    grass.model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2.0f);
    grass.model.translation = center;
    grass.shader = shader_illumination;

    cgp::numarray<cgp::vec3> grass_pos(N_herbe);
    long count = 0;
    while (count < N_herbe)
    {
        cgp::vec3 pos = {rand_interval(-b, b), rand_interval(-a, a), 0};
        if (std::pow(pos.x / b, 2) + std::pow(pos.y / a, 2) <= 1)
        {
            grass_pos[count++] = pos;
        }
    }
    grass.initialize_supplementary_data_on_gpu(grass_pos, 4, 1);
    grass.model.rotation = rotation_transform::from_axis_angle({0, 0, 1}, 3.14f / 2.0f);
    // grass.model.translation = cgp::vec3{-a / 2, b / 3, 0} + center;
    grass.shader = shader_grass;
    // grass.model.scaling = 10;
    bLand0 = true;
    std::cout << "create_object_land0" << std::endl;
}

void create_object_land1(cgp::mesh_drawable &rock1,
                         cgp::vec3 &torch_position,
                         float &torch_min_dist,
                         cgp::vec3 &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float &scale,
                         bool &bLand1)
{
    mesh rock1_mesh = mesh_load_file_obj("assets/blender_obj/Land/land1/land1.obj");
    rock1_mesh.apply_to_position(cgp::mat4::build_rotation_from_axis_angle({1, 0, 0}, 3.14f / 2));
    rock1_mesh.apply_rotation_to_position({0, 0, 1}, -3.14f / 2);
    rock1.initialize_data_on_gpu(rock1_mesh);
    rock1.texture.load_and_initialize_texture_2d_on_gpu("assets/blender_obj/Land/land1/texture_land1.png");
    rock1.model.scaling = scale / 0.05f;
    cgp::vec3 pos = cgp::vec3{-5.0f, -80.0f, -140.0f} * scale + center;
    rock1.model.translation = pos;
    rock1.shader = shader_illumination;

    torch_position = {pos.x + 10, pos.y + 20, -pos.z + 70};
    torch_min_dist = 1000.0f;
    bLand1 = true;
    std::cout << "create_object_land1" << std::endl;
}

void create_object_land2(cgp::mesh_drawable &rock2,
                         cgp::hierarchy_mesh_drawable &house_model_0,
                         cgp::vec3 &painting_position,
                         float &painting_min_dist,
                         cgp::vec3 &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float &scale,
                         bool &bLand2)
{
    rock2.initialize_data_on_gpu(mesh_load_file_obj("assets/blender_obj/Land/land2/land2.obj"));
    rock2.texture.load_and_initialize_texture_2d_on_gpu("assets/blender_obj/Land/land2/texture_land2.png");
    rock2.model.scaling = scale / 0.20f;
    rock2.model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2.0f);
    rock2.model.translation = cgp::vec3{0.0f, 0.0f, -50.0f} + center;
    rock2.shader = shader_illumination;

    house_model_0 = create_house_model_0(15.0f, center, painting_position, painting_min_dist, shader_illumination);
    bLand2 = true;
    std::cout << "create_object_land2" << std::endl;
}

void create_object_land3(cgp::mesh_drawable &rocket_platform,
                         cgp::mesh_drawable &rocket,
                         float height,
                         cgp::vec3 const &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float const &scale,
                         bool &bLand3)
{
    cgp::mesh rocket_mesh;
    cgp::mesh m1;
    cgp::mesh m2;
    cgp::mesh triangle1;
    cgp::mesh triangle2;
    cgp::mesh triangle3;
    cgp::mesh triangle4;
    cgp::mesh cone;

    cgp::mesh c;
    cgp::mesh c1;
    rocket_mesh = mesh_primitive_cylinder(height / 6, {0, 0, 0}, {0, 0, height}, 10, 20, true);
    m1 = mesh_primitive_cylinder(height / 6, {0, 0, 0}, {0, 0, 0.1 * height}, 10, 20, true);
    m2 = mesh_primitive_cylinder(height / 6, {0, 0, 0}, {0, 0, 0.4 * height}, 10, 20, true);

    rocket_mesh.color.fill({128 / 256.0f, 128 / 256.0f, 128 / 256.0f});
    m2.color.fill({128 / 256.0f, 128 / 256.0f, 128 / 256.0f});

    m1.apply_translation_to_position({0, 0, height});
    m2.apply_translation_to_position({0, 0, 1.1 * height});
    c = mesh_primitive_cone(height / 6, height / 5, {0, 0, 0}, {0, 0, 1}, true);
    c.apply_translation_to_position({0, 0, 1.5 * height});

    triangle1 = mesh_primitive_triangle({0, 0, 16 * height / 25}, {10 * height / 25, 0, 0}, {0, 0, 3 * height / 25});
    triangle2 = triangle1;
    triangle3 = triangle1;
    triangle4 = triangle1;
    triangle2.apply_rotation_to_position({0, 0, 1}, 3.14 / 2.0f);
    triangle3.apply_rotation_to_position({0, 0, 1}, 3.14f);
    triangle4.apply_rotation_to_position({0, 0, 1}, 3 * 3.14 / 2.0f);

    c1 = mesh_primitive_cone(height / 6, height / 5, {0, 0, 0}, {0, 0, 1}, true);
    cone = mesh_primitive_cone(height / 6, 2 * height / 5, {0, 0, 0}, {0, 0, 1}, true);
    cone.apply_translation_to_position({0, 0, -2 * height / 25});
    // cone.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/rocket.png");
    triangle1.color.fill({1, 0, 0});
    triangle2.color.fill({1, 0, 0});
    triangle3.color.fill({1, 0, 0});
    triangle4.color.fill({1, 0, 0});
    m1.color.fill({1, 0, 0});
    rocket_mesh.push_back(c);
    rocket_mesh.push_back(triangle1);
    rocket_mesh.push_back(triangle2);
    rocket_mesh.push_back(triangle3);
    rocket_mesh.push_back(triangle4);
    rocket_mesh.push_back(c1);
    rocket_mesh.push_back(cone);
    rocket_mesh.push_back(m1);
    rocket_mesh.push_back(m2);

    rocket_mesh.apply_scaling_to_position(3);
    rocket_mesh.apply_translation_to_position(center + cgp::vec3{0, 0, height / 6});

    rocket.initialize_data_on_gpu(rocket_mesh);
    rocket.material.phong.specular = 0;
    rocket.shader = shader_illumination;
    /////////

    mesh platform_mesh = mesh_primitive_cylinder(15, center + cgp::vec3{0, 0, -3}, center, 2, 10, true);
    platform_mesh.color.fill({124 / 256.0f, 64 / 256.0f, 51 / 256.0f});
    rocket_platform.shader = shader_illumination;

    rocket_platform.initialize_data_on_gpu(platform_mesh);

    ////////
    bLand3 = true;
}

void create_object_water(cgp::mesh_drawable &water,
                         cgp::mesh_drawable &fwater,
                         cgp::mesh_drawable &fishes,
                         long &N_fishes,
                         cgp::mesh_drawable &trees,
                         cgp::opengl_shader_structure &shader_water,
                         cgp::opengl_shader_structure &shader_illumination,
                         cgp::opengl_shader_structure &shader_fishes,
                         bool &bWater)
{
    float l = 1000.0f;
    float wz = -10.0f;
    mesh water_mesh = mesh_primitive_grid_txt({-l / 2, -l / 2, wz}, {l / 2, -l / 2, wz}, {l / 2, l / 2, wz}, {-l / 2, l / 2, wz}, 50, 50, 50);
    // water.material.alpha = 0.5f;
    water.initialize_data_on_gpu(water_mesh);

    water.material.phong.specular = 0;
    water.texture.load_and_initialize_texture_2d_on_gpu("assets/textures_water/texture_water_2.jpg", GL_REPEAT, GL_REPEAT);
    water.shader = shader_water;
    water.material.alpha = 0.7f;

    // mesh fwater_mesh = mesh_primitive_grid_txt({-l / 2, -l / 2, wz - 10}, {l / 2, -l / 2, wz - 10}, {l / 2, l / 2, wz - 10}, {-l / 2, l / 2, wz - 10}, 100, 100, 100);
    float terrain_length = 2000;
    mesh fwater_mesh = create_terrain_mesh(200, terrain_length);
    fwater_mesh.color.fill({105 / 256.0f, 56 / 256.0f, 44 / 256.0f});
    fwater.initialize_data_on_gpu(fwater_mesh);
    fwater.shader = shader_illumination;

    mesh trees_mesh;
    long N_rd_tree = 2000;
    cgp::numarray<cgp::vec3> rd_tree_pos(N_rd_tree);
    long trees_count = 0;
    while (trees_count < N_rd_tree)
    {
        float pos_x = rand_interval(-1000, 1000);
        float pos_y = rand_interval(-1000, 1000);

        float dist = std::sqrt(std::pow(pos_x, 2) + std::pow(pos_y, 2));
        if (dist > 460 && dist < 900)
        {
            mesh rd_tree = create_tree_model_2(8);
            rd_tree.apply_translation_to_position({pos_x, pos_y, evaluate_terrain_height(pos_x, pos_y, terrain_length)});
            trees_mesh.push_back(rd_tree);
            trees_count++;
        }
    }
    trees.initialize_data_on_gpu(trees_mesh);
    // trees.shader = shader_illumination;
    trees.material.phong.specular = 0.0f;

    fishes.initialize_data_on_gpu(mesh_load_file_obj("assets/blender_obj/Fishes/fish_0.obj"));
    fishes.texture.load_and_initialize_texture_2d_on_gpu("assets/blender_obj/Fishes/fish_0_texture.jpg");
    fishes.shader = shader_fishes;

    cgp::numarray<cgp::vec3> fishes_pos(N_fishes);
    long fish_count = 0;
    while (fish_count < N_fishes)
    {
        cgp::vec3 pos = {rand_interval(-l / 10.0f, l / 10.0f), rand_interval(-l / 10.0f, l / 10.0f), 0};
        bool add = true;
        for (cgp::vec3 vec : fishes_pos)
        {
            if (cgp::norm(pos - vec) < 1.0f)
            {
                add = false;
                break;
            }
        }
        if (add)
        {
            fishes_pos[fish_count++] = pos;
        }
    }
    fishes.model.translation = {0, 0, wz - 5.0f};
    fishes.initialize_supplementary_data_on_gpu(fishes_pos, 4, 1);

    bWater = true;
    std::cout << "create_object_water" << std::endl;
}
