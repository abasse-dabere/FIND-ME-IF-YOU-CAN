#include "windmill.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

using namespace cgp;

cgp::hierarchy_mesh_drawable create_windmill(float const &scale,
                                             cgp::opengl_shader_structure const &shader_illumination)
{
    cgp::hierarchy_mesh_drawable m;
    float base_radius = 4.0f;
    float base_height = 10.0f;
    float facteur_centre = ((base_radius + 1.0f) / 3.0f + base_height);
    vec3 base_begin = {0, 0, -facteur_centre};
    vec3 base_end = {0, 0, base_height - facteur_centre};
    cgp::mesh_drawable base;
    base.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(base_radius, base_begin, base_end, 10, 20, true));
    // base.texture.load_and_initialize_texture_2d_on_gpu("assets/mur3.jpeg", GL_REPEAT, GL_REPEAT);
    base.material.color = {128 / 256.0f, 128 / 256.0f, 128 / 256.0f};
    base.material.phong.specular = 0.0f;
    base.shader = shader_illumination;

    float head_radius = base_radius + 1;
    float head_height = head_radius;
    cgp::mesh_drawable head;
    head.initialize_data_on_gpu(cgp::mesh_primitive_cone(head_radius, head_height, base_end, {0, 0, 1}));
    // head.texture.load_and_initialize_texture_2d_on_gpu("assets/paille1.jpeg", GL_REPEAT, GL_REPEAT);
    head.material.color = {102 / 256.0f, 51 / 256.0f, 0.0f};
    head.material.phong.specular = 0.0f;
    head.shader = shader_illumination;

    //   ((base_radius + 1.0f) / 3.0f + base_height)
    float bolt1_radius = 1.0f;
    vec3 bolt1_begin = base_end + vec3{0, 0, head_height / 3};
    vec3 bolt1_end = bolt1_begin + vec3{0, 5, 0};
    cgp::mesh_drawable bolt1;
    bolt1.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(1.0f, bolt1_begin, bolt1_end, 10, 20, true));
    bolt1.texture.load_and_initialize_texture_2d_on_gpu("assets/bois1.png", GL_REPEAT, GL_REPEAT);
    bolt1.material.phong.specular = 0.0f;
    bolt1.shader = shader_illumination;

    // le cylindre qui est cencé tourné
    float bolt2_radius = bolt1_radius / 1.5f;
    vec3 bolt2_begin = bolt1_end;
    vec3 bolt2_end = bolt1_end + vec3{0, 0.5f, 0};
    cgp::mesh_drawable bolt2;
    bolt2.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(bolt2_radius, bolt2_begin, bolt2_end, 10, 20, true));
    bolt2.material.color = vec3{43, 21, 7} / 256.0f;
    bolt2.material.phong.specular = 0.0f;
    bolt2.shader = shader_illumination;

    // le petit cube
    float bolt3_radius = bolt2_radius;
    vec3 bolt3_center = bolt2_end + vec3{0, bolt3_radius / 2, 0};
    cgp::mesh_drawable bolt3;
    bolt3.initialize_data_on_gpu(cgp::mesh_primitive_cube(bolt3_center, bolt3_radius));
    bolt3.texture.load_and_initialize_texture_2d_on_gpu("assets/planche1.jpeg", GL_REPEAT, GL_REPEAT);
    bolt3.material.phong.specular = 0.0f;
    bolt3.shader = shader_illumination;

    float bar_height = 0.75f * base_height;
    float bar_radius = bolt3_radius / 2.0f;
    vec3 bar1_begin = bolt3_center + vec3{-bolt3_radius / 2, 0, 2 * bar_radius};
    vec3 bar1_end = bar1_begin + vec3{bar_height, 0, 0};
    cgp::mesh_drawable bar1;
    bar1.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(bar_radius, bar1_begin, bar1_end, 10, 20, true));
    bar1.texture.load_and_initialize_texture_2d_on_gpu("assets/bois1.png", GL_REPEAT, GL_REPEAT);
    bar1.material.phong.specular = 0.0f;
    bar1.shader = shader_illumination;

    vec3 bar2_begin = bolt3_center + vec3{2 * bar_radius, 0, bolt3_radius / 2};
    vec3 bar2_end = bar2_begin + vec3{0, 0, -bar_height};
    cgp::mesh_drawable bar2;
    bar2.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(bar_radius, bar2_begin, bar2_end, 10, 20, true));
    bar2.texture.load_and_initialize_texture_2d_on_gpu("assets/bois1.png", GL_REPEAT, GL_REPEAT);
    bar2.material.phong.specular = 0.0f;
    bar2.shader = shader_illumination;

    vec3 bar3_begin = bolt3_center + vec3{bolt3_radius / 2, 0, -2 * bar_radius};
    vec3 bar3_end = bar3_begin + vec3{-bar_height, 0, 0};
    cgp::mesh_drawable bar3;
    bar3.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(bar_radius, bar3_begin, bar3_end, 10, 20, true));
    bar3.texture.load_and_initialize_texture_2d_on_gpu("assets/bois1.png", GL_REPEAT, GL_REPEAT);
    bar3.material.phong.specular = 0.0f;
    bar3.shader = shader_illumination;

    vec3 bar4_begin = bolt3_center + vec3{-2 * bar_radius, 0, -bolt3_radius / 2};
    vec3 bar4_end = bar4_begin + vec3{0, 0, bar_height};
    cgp::mesh_drawable bar4;
    bar4.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(bar_radius, bar4_begin, bar4_end, 10, 20, true));
    bar4.texture.load_and_initialize_texture_2d_on_gpu("assets/bois1.png", GL_REPEAT, GL_REPEAT);
    bar4.material.phong.specular = 0.0f;
    bar4.shader = shader_illumination;

    cgp::mesh_drawable helix1;
    vec3 offset1 = bar1_begin + vec3{0, bar_radius, 0};
    helix1.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle(offset1 + vec3{1, 0, 0}, offset1 + vec3{1, 0, 3}, offset1 + vec3{bar_height - 0.25f, 0, 3}, offset1 + vec3{bar_height - 0.25f, 0, 0}));
    helix1.texture.load_and_initialize_texture_2d_on_gpu("assets/planche2.jpg", GL_REPEAT, GL_REPEAT);
    helix1.material.phong.specular = 0.0f;
    helix1.shader = shader_illumination;

    cgp::mesh_drawable helix2;
    vec3 offset2 = bar2_begin + vec3{0, bar_radius, 0};
    helix2.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle(offset2 + vec3{0, 0, -1}, offset2 + vec3{3, 0, -1}, offset2 + vec3{3, 0, -bar_height + 0.25f}, offset2 + vec3{0, 0, -bar_height + 0.25f}));
    helix2.texture.load_and_initialize_texture_2d_on_gpu("assets/planche2.jpg", GL_REPEAT, GL_REPEAT);
    helix2.material.phong.specular = 0.0f;
    helix2.shader = shader_illumination;

    cgp::mesh_drawable helix3;
    vec3 offset3 = bar3_begin + vec3{0, bar_radius, 0};
    helix3.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle(offset3 + vec3{-1, 0, 0}, offset3 + vec3{-1, 0, -3}, offset3 + vec3{-bar_height + 0.25f, 0, -3}, offset3 + vec3{-bar_height + 0.25f, 0, 0}));
    helix3.texture.load_and_initialize_texture_2d_on_gpu("assets/planche2.jpg", GL_REPEAT, GL_REPEAT);
    helix3.material.phong.specular = 0.0f;
    helix3.shader = shader_illumination;

    cgp::mesh_drawable helix4;
    vec3 offset4 = bar4_begin + vec3{0, bar_radius, 0};
    helix4.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle(offset4 + vec3{0, 0, 1}, offset4 + vec3{-3, 0, 1}, offset4 + vec3{-3, 0, bar_height - 0.25f}, offset4 + vec3{0, 0, bar_height - 0.25f}));
    helix4.texture.load_and_initialize_texture_2d_on_gpu("assets/planche2.jpg", GL_REPEAT, GL_REPEAT);
    helix4.material.phong.specular = 0.0f;
    helix4.shader = shader_illumination;

    m.add(base, "Base");
    m.add(head, "head", "Base");
    m.add(bolt1, "bolt1", "head");
    m.add(bolt2, "bolt2", "bolt1");
    m.add(bolt3, "bolt3", "bolt2");
    m.add(bar1, "bar1", "bolt3");
    m.add(helix1, "helix1", "bar1");
    m.add(bar2, "bar2", "bolt3");
    m.add(helix2, "helix2", "bar2");
    m.add(bar3, "bar3", "bolt3");
    m.add(helix3, "helix3", "bar3");
    m.add(bar4, "bar4", "bolt3");
    m.add(helix4, "helix4", "bar4");
    m["Base"].transform_local.translation = cgp::vec3{0, 0, 2 * facteur_centre * scale};
    m["Base"].transform_local.scaling = 2 * scale;
    return m;
}