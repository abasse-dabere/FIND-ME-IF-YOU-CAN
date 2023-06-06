#include "interpolation.hpp"

using namespace cgp;

void cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, cgp::vec3 const &p0, cgp::vec3 const &p1, cgp::vec3 const &p2, cgp::vec3 const &p3, float K, cgp::vec3 &pos, cgp::vec3 &dr);
int find_index_of_interval(float t, std::vector<float> const &intervals);
cgp::vec3 position_p(std::vector<cgp::vec3> &key_positions);

void interpolation(long &curr_idx, float t, std::vector<cgp::vec3> &key_positions, std::vector<float> &key_times, cgp::vec3 &pos, cgp::vec3 &der)
{
    int idx = find_index_of_interval(t, key_times);

    float t0 = key_times[idx - 1];
    float t1 = key_times[idx]; // t_i
    float t2 = key_times[idx + 1];
    float t3 = key_times[idx + 2];

    cgp::vec3 const &p0 = key_positions[idx - 1];
    cgp::vec3 const &p1 = key_positions[idx]; // p_i
    cgp::vec3 const &p2 = key_positions[idx + 1];
    cgp::vec3 const &p3 = key_positions[idx + 2];

    if (idx > curr_idx)
    {
        cgp::vec3 new_p = position_p(key_positions);

        key_positions.push_back(new_p);

        float dist = cgp::norm(key_positions[key_positions.size() - 1] - key_positions[key_positions.size() - 2]);
        float velocity = 15.0f;

        float new_t = key_times[key_times.size() - 1] + dist / velocity;
        key_times.push_back(new_t);

        curr_idx = idx;
    }

    cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, 0.5, pos, der);

    if (idx > 3)
    {
        key_positions.erase(key_positions.begin());
        key_times.erase(key_times.begin());

        curr_idx = idx - 1;
    }
}

cgp::vec3 linear_interpolation(float t, float t1, float t2, cgp::vec3 const &p1, cgp::vec3 const &p2)
{
    float const alpha = (t - t1) / (t2 - t1);
    cgp::vec3 const p = (1 - alpha) * p1 + alpha * p2;

    return p;
}

cgp::vec3 position_p(std::vector<cgp::vec3> &key_positions)
{
    cgp::vec3 A = key_positions[key_positions.size() - 1];

    cgp::vec3 p;
    p.z = A.z;
    do
    {
        float signx = rand_interval(-1, 1) > 0 ? 1 : -1;
        float signy = rand_interval(-1, 1) > 0 ? 1 : -1;
        p.x = A.x + rand_interval(30, 40) * signx;
        p.y = A.y + rand_interval(30, 40) * signy;
    } while (cgp::norm(p) > 400);

    return p;
}

/*
cgp::vec3 position_p(std::vector<cgp::vec3> &key_positions)
{
    cgp::vec3 p;

    cgp::vec3 u = key_positions[key_positions.size() - 1] - key_positions[key_positions.size() - 2];
    cgp::vec3 A = key_positions[key_positions.size() - 1];
    long min_dist = 20;
    long max_dist = 100;
    do
    {
        float dist = rand_interval(min_dist, max_dist);
        float alpha = rand_interval(-60, 60) / 180 * 3.14f;
        float beta = std::atan(u.y / u.x);
        // std::cout << " beta :" << beta << std::endl;
        p.x = dist * std::cos(alpha + beta) + A.x;
        // std::cout << " cos :" << std::cos(alpha + beta) << std::endl;
        // std::cout << " sin :" << std::sin(alpha + beta) << std::endl;
        p.y = dist * std::sin(alpha + beta) + A.y;
        p.z = A.z;
    } while (cgp::norm(p) > 400);

    return p;
}*/

int find_index_of_interval(float t, std::vector<float> const &intervals)
{
    int const N = intervals.size();

    int k = 0;
    while (intervals[k + 1] < t)
        ++k;
    return k;
}

void cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, cgp::vec3 const &p0, cgp::vec3 const &p1, cgp::vec3 const &p2, cgp::vec3 const &p3, float K, cgp::vec3 &pos, cgp::vec3 &der)
{
    float s = (t - t1) / (t2 - t1);
    cgp::vec3 const di = (2.0f * K) * (p2 - p0) / (t2 - t0);
    cgp::vec3 const dii = (2.0f * K) * (p3 - p1) / (t3 - t1);

    pos = (2 * std::pow(s, 3) - 3 * std::pow(s, 2) + 1) * p1;
    pos += (std::pow(s, 3) - 2 * std::pow(s, 2) + s) * di;
    pos += (-2 * std::pow(s, 3) + 3 * std::pow(s, 2)) * p2;
    pos += (std::pow(s, 3) - std::pow(s, 2)) * dii;

    float dsdt = 1 / (t2 - t1);
    der = (6 * std::pow(s, 2) - 6 * s) * p1;
    der += (3 * std::pow(s, 2) - 4 * s + 1) * di;
    der += (-6 * std::pow(s, 2) + 6 * s) * p2;
    der += (3 * std::pow(s, 2) - 2 * s) * dii;
    der *= dsdt;
}