#include "custom_primitive.hpp"

using namespace cgp;

static numarray<uint3> connectivity_grid(size_t Nu, size_t Nv)
{
    numarray<uint3> connectivity;
    for (size_t ku = 0; ku < Nu - 1; ++ku)
    {
        for (size_t kv = 0; kv < Nv - 1; ++kv)
        {
            unsigned int k00 = static_cast<unsigned int>(kv + Nv * ku);
            unsigned int k10 = static_cast<unsigned int>(kv + 1 + Nv * ku);
            unsigned int k01 = static_cast<unsigned int>(kv + Nv * (ku + 1));
            unsigned int k11 = static_cast<unsigned int>(kv + 1 + Nv * (ku + 1));

            connectivity.push_back(uint3{k00, k10, k11});
            connectivity.push_back(uint3{k00, k11, k01});
        }
    }
    return connectivity;
}

cgp::mesh create_demi_sphere_txt(long txt, float radius, cgp::vec3 center, int Nu, int Nv)
{
    assert_cgp(radius > 0, "Sphere radius should be > 0");
    assert_cgp(Nu > 2 && Nv > 2, "Sphere samples should be > 2");

    mesh shape;
    for (size_t ku = 0; ku < size_t(Nu); ++ku)
    {
        for (size_t kv = 0; kv < size_t(Nv); ++kv)
        {
            float const u = ku / (Nu - 1.0f);
            float const alpha = kv / (Nv - 1.0f);
            float const v = 1.0f / (Nv + 1.0f) * (1 - alpha) + alpha * Nv / (Nv + 1.0f);

            float const theta = Pi * (u - 0.5f);
            float const phi = Pi * (v - 0.5f);

            // spherical coordinates
            cgp::vec3 const n = {
                std::cos(phi) * std::cos(theta),
                std::cos(phi) * std::sin(theta),
                std::sin(phi)};
            cgp::vec3 const p = radius * n + center;
            vec2 const uv = {u * txt, v * txt};

            shape.position.push_back(p);
            shape.normal.push_back(n);
            shape.uv.push_back(uv);
        }
    }

    shape.connectivity = connectivity_grid(Nu, Nv);

    // poles
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
    {
        shape.position.push_back(center + radius * cgp::vec3{0, 0, -1.0f});
        shape.normal.push_back(cgp::vec3{0, 0, -1.0f});
        shape.uv.push_back({ku / (Nu - 1.0f), 0.0f});
    }
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
        shape.connectivity.push_back({Nu * Nv + ku, Nv * ku, Nv * (ku + 1)});

    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
    {
        shape.position.push_back(center + radius * cgp::vec3{0, 0, 1.0f});
        shape.normal.push_back(cgp::vec3{0, 0, 1.0f});
        shape.uv.push_back({ku / (Nu - 1.0f), 1.0f});
    }
    for (size_t ku = 0; ku < size_t(Nu - 1); ++ku)
        shape.connectivity.push_back({Nu * Nv + Nu - 1 + ku, Nv - 1 + Nv * (ku + 1), Nv - 1 + Nv * ku});

    shape.fill_empty_field();
    shape.flip_connectivity();

    shape.apply_rotation_to_position({0, 1, 0}, 3.14f / 2.0f);

    return shape;
}

cgp::mesh mesh_primitive_grid_txt(cgp::vec3 const &p00, cgp::vec3 const &p10, cgp::vec3 const &p11, cgp::vec3 const &p01, int Nu, int Nv, long txt)
{
    assert_cgp(Nu > 1, "Grid sample must be >1");
    assert_cgp(Nv > 1, "Grid sample must be >1");

    mesh shape;
    for (size_t ku = 0; ku < size_t(Nu); ++ku)
    {
        for (size_t kv = 0; kv < size_t(Nv); ++kv)
        {

            float const u = ku / (Nu - 1.0f);
            float const v = kv / (Nv - 1.0f);

            cgp::vec3 const p = (1 - u) * (1 - v) * p00 + u * (1 - v) * p10 + u * v * p11 + (1 - u) * v * p01;

            cgp::vec3 const dpdu = (1 - u) * (-p00 + p01) + u * (-p10 + p11);
            cgp::vec3 const dpdv = (1 - v) * (-p00 + p10) + v * (p11 - p01);
            cgp::vec3 const n = normalize(cross(dpdv, dpdu));
            vec2 const uv = {u * txt, v * txt};

            shape.position.push_back(p);
            shape.normal.push_back(n);
            shape.uv.push_back(uv);
        }
    }
    shape.connectivity = connectivity_grid(Nu, Nv);
    shape.fill_empty_field();
    shape.flip_connectivity();
    return shape;
}