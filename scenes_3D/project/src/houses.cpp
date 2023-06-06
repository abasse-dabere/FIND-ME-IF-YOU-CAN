#include "tree.hpp"
#include "custom_primitive.hpp"

using namespace cgp;

cgp::hierarchy_mesh_drawable create_house_model_0(float height,
                                                  cgp::vec3 &center,
                                                  cgp::vec3 &painting_position,
                                                  float &painting_min_dist,
                                                  cgp::opengl_shader_structure &shader_illumination)
{
    cgp::hierarchy_mesh_drawable m;
    height = 20.0f;

    float hx = 9.0f * height / 10.0f;
    float hy = 0.2f * height / 10.0f;
    float hz = height;
    float cube_h = height / 2.0f;
    float offx = center.x;
    float offy = height / 2 + center.y;
    float offz = cube_h + (3 * hz / 8.0f * std::cos(3.14f / 6.0f)) + center.z + height / 10.0f;
    cgp::mat4 pos = cgp::mat4::build_translation({offx, offy, offz}) * cgp::mat4::build_rotation_from_axis_angle({0, 0, 1}, 2 * 3.14f / 3.0f);

    mesh toit = mesh_primitive_cube();
    toit.apply_to_position(cgp::mat4::build_rotation_from_axis_angle({1, 0, 0}, 3.14f / 6.0f) * cgp::mat4::build_scaling({hx, hy, hz}));
    // toit.apply_to_position(cgp::mat4::build_rotation_from_axis_angle({1, 0, 0}, 3.14f / 6.0f));

    mesh toit2 = toit;
    toit2.apply_to_position(cgp::mat4::build_translation({0, -hz / 2.0f, 0}) * cgp::mat4::build_rotation_from_axis_angle({1, 0, 0}, -3.14f / 3.0f));
    // toit2.apply_to_position(cgp::mat4::build_translation({0, -hz / 2.0f, 0}));
    toit.push_back(toit2);
    toit.apply_to_position(pos);

    cgp::mesh_drawable toit_md;
    toit_md.initialize_data_on_gpu(toit);
    toit_md.material.color = {102 / 256.0f, 51 / 256.0f, 0.0f};
    // toit_md.texture.load_and_initialize_texture_2d_on_gpu("assets/houses/texture_tile_0.png", GL_REPEAT, GL_REPEAT);
    toit_md.material.phong.specular = 0.0f;
    toit_md.shader = shader_illumination;
    m.add(toit_md, "Toit");

    float offset = hx / 20.0f;
    cgp::vec3 f1_p0 = {hx / 2.0f - offset, -hz / 4.0f, hz / 2.0f * std::cos(3.14f / 6.0f)};
    cgp::vec3 f1_p1 = {hx / 2.0f - offset, -(hz / 2.0f + 3 * hz / 8.0f * std::sin(3.14f / 6.0f)), -(3 * hz / 8.0f * std::cos(3.14f / 6.0f))};
    cgp::vec3 f1_p2 = {hx / 2.0f - offset, 3 * hz / 8.0f * std::sin(3.14f / 6.0f), -(3 * hz / 8.0f * std::cos(3.14f / 6.0f))};
    mesh triangle0 = mesh_primitive_triangle(f1_p0, f1_p1, f1_p2);

    cgp::vec3 back_tr_off = {-2 * (hx / 2.0f - offset), 0, 0};
    mesh triangle1 = mesh_primitive_triangle(f1_p0 + back_tr_off, f1_p2 + back_tr_off, f1_p1 + back_tr_off);
    triangle1.push_back(triangle0);
    triangle1.apply_to_position(pos);

    cgp::mesh_drawable triangle_md;
    triangle_md.initialize_data_on_gpu(triangle1);
    triangle_md.texture.load_and_initialize_texture_2d_on_gpu("assets/houses/texture_tile_0.png", GL_REPEAT, GL_REPEAT);
    triangle_md.material.phong.specular = 0.0f;
    triangle_md.shader = shader_illumination;
    m.add(triangle_md, "triangle_md", "Toit");

    cgp::vec3 c_p0 = f1_p1;
    cgp::vec3 c_p1 = f1_p1 + cgp::vec3{0, 0, -cube_h};
    cgp::vec3 c_p2 = f1_p2 + cgp::vec3{0, 0, -cube_h};
    cgp::vec3 c_p3 = f1_p2;
    cgp::vec3 c_p4 = c_p3 + back_tr_off;
    cgp::vec3 c_p5 = c_p2 + back_tr_off;
    cgp::vec3 c_p6 = c_p1 + back_tr_off;
    cgp::vec3 c_p7 = c_p0 + back_tr_off;
    mesh front_quad = mesh_primitive_quadrangle(c_p0, c_p1, c_p2, c_p3);
    front_quad.apply_to_position(pos);
    cgp::mesh_drawable front_quand_md;
    front_quand_md.initialize_data_on_gpu(front_quad);
    front_quand_md.texture.load_and_initialize_texture_2d_on_gpu("assets/houses/texture_tile_door.png", GL_REPEAT, GL_REPEAT);
    front_quand_md.shader = shader_illumination;
    m.add(front_quand_md, "front_quand_md", "Toit");

    mesh right_quad = mesh_primitive_quadrangle(c_p3, c_p2, c_p5, c_p4);
    mesh left_quad = mesh_primitive_quadrangle(c_p0, c_p7, c_p6, c_p1);
    mesh back_quad = mesh_primitive_quadrangle(c_p4, c_p5, c_p6, c_p7);
    back_quad.push_back(right_quad);
    back_quad.push_back(left_quad);
    back_quad.apply_to_position(pos);

    cgp::mesh_drawable back_quand_md;
    back_quand_md.initialize_data_on_gpu(back_quad);
    back_quand_md.texture.load_and_initialize_texture_2d_on_gpu("assets/houses/texture_tile_0.png", GL_REPEAT, GL_REPEAT);
    back_quand_md.material.phong.specular = 0.0f;
    back_quand_md.shader = shader_illumination;
    m.add(back_quand_md, "back_quand_md", "Toit");

    float pratio = 1 / 4.0f;
    cgp::mesh painting = mesh_primitive_quadrangle(c_p6, c_p5, c_p4, c_p7);
    painting.apply_to_position(cgp::mat4::build_translation({-cube_h / 2, -cube_h / 2, -cube_h / 2}) * cgp::mat4::build_scaling({pratio, pratio, pratio * 2}));
    painting.apply_to_position(pos);
    cgp::mesh_drawable painting_md;
    painting_md.initialize_data_on_gpu(painting);
    painting_md.texture.load_and_initialize_texture_2d_on_gpu("assets/textures/painting.jpg", GL_REPEAT, GL_REPEAT);
    m.add(painting_md, "painting_md", "Toit");
    painting_position = (painting.position[0] + painting.position[2]) / 2;
    painting_min_dist = cgp::norm(painting_position - front_quad.position[0]);

    float p_height = hz / 20.0f;
    float p_lenght = hz * 5 / 4.0f;
    mesh platform = mesh_primitive_cube();
    platform.color.fill({124 / 256.0f, 64 / 256.0f, 51 / 256.0f});
    platform.apply_to_position(cgp::mat4::build_scaling({p_lenght, p_lenght, p_height}));
    platform.apply_to_position(cgp::mat4::build_translation({0, -height / 4.0f, c_p2.z}));
    platform.apply_to_position(pos);

    cgp::mesh_drawable platform_md;
    platform_md.initialize_data_on_gpu(platform);
    // platform_md.texture.load_and_initialize_texture_2d_on_gpu("assets/houses/texture_tile_0.png", GL_REPEAT, GL_REPEAT);
    platform_md.material.phong.specular = 0.0f;
    platform_md.shader = shader_illumination;
    m.add(platform_md, "platform_md", "Toit");

    return m;
}

/*
mesh ms = mesh_primitive_sphere(1.0f, positions_painting[4]);
    mesh_drawable md;
    md.initialize_data_on_gpu(ms);
    m.add(md, "md", "Toit");

    positions_painting.push_back(front_quad.position[0]);                            // n c_p0
    positions_painting.push_back(front_quad.position[3]);                            // n c_p3
    positions_painting.push_back(back_quad.position[0]);                             // n c_p4
    positions_painting.push_back(back_quad.position[3]);                             // n c_p7

*/