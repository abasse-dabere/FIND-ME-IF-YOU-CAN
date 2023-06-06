#include "rock.hpp"
#include "custom_primitive.hpp"

using namespace cgp;

cgp::mesh_drawable create_rock_model_0(float &a,
                                       float &b,
                                       float &h,
                                       cgp::vec3 const &center,
                                       cgp::opengl_shader_structure const &shader_illumination)
{
    cgp::mesh_drawable m;

    float decl = a / 30.0f;
    float platform_h = 4 * 0.04f;
    float sphere_r = a / 10.0f;

    long Nu = 9;
    long Nv = 4;
    cgp::mesh ovale = create_demi_sphere_txt(10, 1, {0, 0, 0}, Nu, Nv);
    ovale.apply_to_position(cgp::mat4::build_scaling({a, b, h}));
    ovale.color.fill({124 / 256.0f, 64 / 256.0f, 51 / 256.0f});
    // ovale_md.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-roche-marron-rouge.jpeg", GL_REPEAT, GL_REPEAT);

    cgp::mesh platform = mesh_primitive_cylinder(1, cgp::vec3{0, 0, -platform_h / 2}, {0, 0, platform_h}, 50, 30, true);
    platform.apply_to_position(cgp::mat4::build_scaling({a + decl, b + decl, 1}));
    platform.color.fill({11 / 256.0f, 209 / 256.0f, 25 / 256.0f});
    ovale.push_back(platform);

    //////
    cgp::mesh sphere = mesh_primitive_sphere(sphere_r);
    long N_pos = 40;
    long count_n = 0;
    cgp::mesh spheres;
    std::vector<cgp::vec3> all_pos;
    while (count_n < N_pos)
    {
        long idx = std::ceil(rand_interval(0, Nu * Nv));
        cgp::vec3 pos = ovale.position[idx];
        float bruitx = rand_interval(0, sphere_r * pos.x / std::abs(pos.x));
        float bruity = rand_interval(0, sphere_r * pos.y / std::abs(pos.y));
        float bruitz = rand_interval(0, sphere_r * pos.z / std::abs(pos.z));
        pos = pos + cgp::vec3{bruitx, bruity, bruitz};
        bool add = true;
        for (cgp::vec3 vec : all_pos)
        {
            if (cgp::norm(vec - pos) < 3 * sphere_r)
            {
                add = false;
                break;
            }
        }
        if (add)
        {
            all_pos.push_back(pos);
            cgp::mesh sphere2 = sphere;
            float scale = rand_interval(0.5f, 1.5f);
            sphere2.apply_to_position(cgp::mat4::build_scaling(cgp::vec3{scale, scale, scale}));
            sphere2.apply_translation_to_position(pos);
            spheres.push_back(sphere2);
            count_n++;
        }
    }
    spheres.color.fill({124 / 256.0f, 64 / 256.0f, 51 / 256.0f});
    ovale.push_back(spheres);
    /////

    m.initialize_data_on_gpu(ovale);
    m.model.translation = center;
    m.shader = shader_illumination;

    return m;
}

cgp::mesh create_rock_model_1(float r, float h)
{
    cgp::mesh m;
    float rf = r / 5.0f;
    float N = 10.0f;
    float delat_r = (rf - r) / (N - 1.0f);
    cgp::mesh model0 = mesh_primitive_cylinder(r, cgp::vec3{0, 0, 0}, cgp::vec3{0, 0, h / N}, 2, 10, true);
    model0.color.fill({0, 153 / 256.0f, 0});
    m.push_back(model0);
    for (size_t i = 1; i < N; i++)
    {
        float nr = i * (rf - r) / (N - 1.0f) + r;
        float alpha = rand_interval(0, 2 * 3.14f);
        cgp::vec3 ncenter = cgp::vec3{delat_r * std::cos(alpha), delat_r * std::sin(alpha), h * i / N};
        cgp::mesh platform = mesh_primitive_cylinder(nr, ncenter, ncenter + cgp::vec3{0, 0, h / N}, 2, 15, true);
        // platform.apply_to_position(cgp::mat4::build_translation({0, 0, h * i / N}) + cgp::mat4::build_scaling({nr / r, nr / r, 1}));
        platform.color.fill({0.6f, 0.3f, 0.0f});
        m.push_back(platform);
    }
    m.apply_rotation_to_position(cgp::vec3{0, 1, 0}, 3.14f);
    return m;
}

/*

cgp::hierarchy_mesh_drawable m;

    float decl = a / 30.0f;
    float platform_h = 4 * 0.04f;
    float sphere_r = a / 10.0f;

    cgp::mesh ovale = create_demi_sphere();
    ovale.apply_to_position(cgp::mat4::build_scaling({a, b, h}) + cgp::mat4::build_translation({0, 0, -platform_h}));

    cgp::mesh_drawable ovale_md;
    ovale_md.initialize_data_on_gpu(ovale);
    ovale_md.texture.load_and_initialize_texture_2d_on_gpu("assets/textures_rock/texture_rock_0.jpg", GL_REPEAT, GL_REPEAT);
    ovale_md.material.phong.specular = 0.0f;
    ovale_md.shader = shader_illumination;

    cgp::mesh platform = mesh_primitive_cylinder(1, {0, 0, 0}, cgp::vec3{0, 0, -platform_h}, 50, 30, true);
    platform.apply_to_position(cgp::mat4::build_scaling({a + decl, b + decl, 1}));
    platform.color.fill({0, 153 / 256.0f, 0});
    cgp::mesh_drawable platform_md;
    platform_md.initialize_data_on_gpu(platform);
    platform_md.material.phong.specular = 0.0f;

    m.add(ovale_md, "Ovale");
    m.add(platform_md, "Platform", "Ovale");

    return m;

 long N_pos = 20;
    long count_n = 0;
    cgp::mesh spheres;
    cgp::mesh sphere = mesh_primitive_sphere(sphere_r);
    std::vector<cgp::vec3> all_pos;
    while (count_n < N_pos)
    {
        long idx = std::ceil(rand_interval(0, Nu * Nv));
        cgp::vec3 pos = ovale.position[idx];
        float bruitx = rand_interval(0, sphere_r * pos.x / std::abs(pos.x));
        float bruity = rand_interval(0, sphere_r * pos.y / std::abs(pos.y));
        float bruitz = rand_interval(0, sphere_r * pos.z / std::abs(pos.z));
        pos = pos + cgp::vec3{bruitx, bruity, bruitz};
        bool add = true;
        for (cgp::vec3 vec : all_pos)
        {
            if (cgp::norm(vec - pos) < 3 * sphere_r)
            {
                add = false;
                break;
            }
        }
        if (add)
        {
            all_pos.push_back(pos);
            cgp::mesh sphere2 = sphere;
            float scale = rand_interval(0.5f, 1.5f);
            sphere2.apply_to_position(cgp::mat4::build_scaling(cgp::vec3{scale, scale, scale}));
            sphere2.apply_translation_to_position(pos);
            spheres.push_back(sphere2);
            count_n++;
        }
    }

    cgp::mesh_drawable spheres_md;
    spheres_md.initialize_data_on_gpu(spheres);
    spheres_md.texture.load_and_initialize_texture_2d_on_gpu("assets/texture-roche-marron-rouge.jpeg");
    spheres_md.material.phong.specular = 0.0f;
    spheres_md.shader = shader_illumination;
    m.add(spheres_md, "spheres", "Ovale");

*/