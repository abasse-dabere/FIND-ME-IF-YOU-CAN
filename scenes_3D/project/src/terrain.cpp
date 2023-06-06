
#include "terrain.hpp"

using namespace cgp;

perlin_noise_parameters parameters;

float evaluate_terrain_height(float x, float y, float terrain_length)
{
    float z = 0.0f;
    float h = 60.0f;
    float p = 475.0f;
    float sigma = 40.0f;

    float dist = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    z = h * std::exp(-std::pow(dist - (p + 10), 2) / (2 * std::pow(sigma, 2)));
    z += 1.2f * h * std::exp(-std::pow(dist - (p + 150), 2) / (2 * std::pow(sigma, 2)));

    float u = x / terrain_length + 0.5f;
    float v = y / terrain_length + 0.5f;
    float const noise = noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);

    return z * noise - 20.0f;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.uv.resize(N * N);

    // Fill terrain geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            float z = evaluate_terrain_height(x, y, terrain_length);

            // Store vertex coordinates
            terrain.position[kv + N * ku] = {x, y, z};
            terrain.uv[kv + N * ku] = {u * 10, v * 10};
            // terrain.color[kv + N * ku] = 0.3f * vec3(0, 0.5f, 0) + 0.7f * noise * vec3(1, 1, 1);
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = {idx, idx + 1 + N, idx + 1};
            uint3 triangle_2 = {idx, idx + N, idx + 1 + N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();
    terrain.normal_update();
    return terrain;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length)
{
    float min_dist = 0.5f; // plus grand que le rayon des feuillages, attention à ne pas prendre de grandes valeurs
    float offset = 0.2f;   // pour ne pas qu'un arbre se retrouve au bord
    std::vector<cgp::vec3> rand_pos;
    rand_pos.resize(N);
    for (int i = 0; i < N; i++)
    {
        float x = rand_interval(-terrain_length / 2 + offset, terrain_length / 2 - offset);
        float y = rand_interval(-terrain_length / 2 + offset, terrain_length / 2 - offset);
        while (!dont_intersect(x, y, min_dist, rand_pos, i))
        {
            x = rand_interval(-terrain_length / 2 + offset, terrain_length / 2 - offset);
            y = rand_interval(-terrain_length / 2 + offset, terrain_length / 2 - offset);
        }
        float z = evaluate_terrain_height(x, y, terrain_length);
        rand_pos[i] = {x, y, z};
    }
    return rand_pos;
}

bool dont_intersect(float x, float y, float min_dist, std::vector<cgp::vec3> &vec, int fin)
{
    for (int i = 0; i < fin; i++)
    {
        float dist = std::sqrt(std::pow(x - vec[i].x, 2) + std::pow(y - vec[i].y, 2));
        if (dist <= min_dist)
            return false;
    }
    return true;
}