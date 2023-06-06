#include "birds_structure.hpp"

using namespace cgp;
void create_bird(cgp::hierarchy_mesh_drawable &bird_md);

void birds_structure::initialize()
{
	create_bird(bird_md);
	float h = rand_interval(80, 100);
	key_positions = {{-1, 1, h}, {2, 2, h}, {-1, 1, h}, {2, 2, h}, {1.5, -1, h}, {1.5, -1, h}, {-1, -0.5, h}};
	key_times = {-2.0f, -1.0f, 0.0f, 3.0f, 4.5f, 6.0f, 8.0f};
}

// void birds_structure::initialize()
// {
// 	create_bird(bird_md);
// 	std::vector<vec3> key_positions;
// 	std::vector<float> key_times;

// 	cgp::vec3 p0 = {rand_interval(-10, 10), rand_interval(-10, 10), rand_interval(90, 150)};
// 	key_positions.push_back(p0);
// 	key_times.push_back(0);

// 	float velocity = 40.0f;
// 	for (size_t i = 0; i < 4; i++)
// 	{
// 		float x = rand_interval(-10, 10);
// 		float y = rand_interval(-10, 10);
// 		cgp::vec3 p = key_positions[key_positions.size() - 1] + cgp::vec3{x, y, 0};
// 		key_positions.push_back(p);
// 		key_times.push_back(key_times[key_times.size() - 1] + cgp::norm(cgp::vec3{x, y, 0}) / velocity);
// 	}

// 	bird_kf.initialize(key_positions, key_times);
// }

void birds_structure::display(environment_structure &environment, float time)
{
	interpolation(curr_idx, time, key_positions, key_times, pos, der);
	if (std::abs(der.x) < 100 && std::abs(der.x) > 0.001 && std::abs(der.y) < 100 && std::abs(der.y) > 0.001)
		bird_md["base"].transform_local.rotation = rotation_transform::from_vector_transform({1, 0, 0}, cgp::normalize(cgp::vec3{der.x, der.y, 0}));

	bird_md["aile_g0"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 0.5f * std::sin(time * 5));
	bird_md["aile_g1"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 0.5f * std::sin(time * 5));
	bird_md["aile_g1"].transform_local.translation = {0, 2.0f, 0};

	bird_md["aile_d0"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, -0.5f * std::sin(time * 5));
	bird_md["aile_d1"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, -0.5f * std::sin(time * 5));
	bird_md["aile_d1"].transform_local.translation = {0, -2.0f, 0};

	bird_md["base"].transform_local.translation = pos;
	bird_md.update_local_to_global_coordinates();

	draw(bird_md, environment);
}

void create_bird(cgp::hierarchy_mesh_drawable &bird_md)
{
	mesh base_mesh = mesh_primitive_sphere();
	mesh_drawable base;
	base.initialize_data_on_gpu(base_mesh);
	float sx = rand_interval(1.5, 3);
	base.model.scaling_xyz = {sx, 1, 1};
	float cl = rand_interval();
	cgp::vec3 bcolor = {1, 1, 1};
	if (cl < 0.25f)
		bcolor = {124 / 256.0f, 64 / 256.0f, 51 / 256.0f};
	base.material.color = bcolor;
	base.material.phong.specular = 0.0f;

	float ln_aile0 = 2.5 * sx / 2;
	float lg_aile0 = 2.0;
	mesh aile_g0_mesh = mesh_primitive_quadrangle({ln_aile0 / 2, 0, 0}, {ln_aile0 / 2, lg_aile0, 0}, {-ln_aile0 / 2, lg_aile0, 0}, {-ln_aile0 / 2, 0, 0});
	mesh_drawable aile_g0;
	aile_g0.initialize_data_on_gpu(aile_g0_mesh);
	aile_g0.material.color = bcolor;
	aile_g0.material.phong.specular = 0.0f;

	float ln_aile1 = 1.0;
	mesh aile_g1_mesh = mesh_primitive_quadrangle({ln_aile0 / 2, 0, 0}, {ln_aile0 / 4, ln_aile1, 0}, {-ln_aile0 / 3, ln_aile1, 0}, {-ln_aile0 / 2, 0, 0});
	mesh_drawable aile_g1;
	aile_g1.initialize_data_on_gpu(aile_g1_mesh);
	aile_g1.material.color = bcolor;
	aile_g1.material.phong.specular = 0.0f;

	mesh aile_d0_mesh = mesh_primitive_quadrangle({-ln_aile0 / 2, 0, 0}, {-ln_aile0 / 2, -lg_aile0, 0}, {ln_aile0 / 2, -lg_aile0, 0}, {ln_aile0 / 2, 0, 0});
	mesh_drawable aile_d0;
	aile_d0.initialize_data_on_gpu(aile_d0_mesh);
	aile_d0.material.color = bcolor;
	aile_d0.material.phong.specular = 0.0f;

	mesh aile_d1_mesh = mesh_primitive_quadrangle({ln_aile0 / 2, 0, 0}, {-ln_aile0 / 2, 0, 0}, {-ln_aile0 / 3, -ln_aile1, 0}, {ln_aile0 / 4, -ln_aile1, 0});
	mesh_drawable aile_d1;
	aile_d1.initialize_data_on_gpu(aile_d1_mesh);
	aile_d1.material.color = bcolor;
	aile_d1.material.phong.specular = 0.0f;

	mesh head_mesh = mesh_primitive_sphere(0.85);
	mesh_drawable head;
	head.initialize_data_on_gpu(head_mesh);
	head.model.translation = {sx, 0, 0};
	head.material.phong.specular = 0.0f;

	mesh beck_mesh = mesh_primitive_cone(0.4, 0.7f);
	mesh_drawable beck;
	beck.initialize_data_on_gpu(beck_mesh);
	beck.model.translation = {2.7, 0, -0.2};
	beck.model.rotation = cgp::rotation_transform::from_axis_angle({0, 1, 0}, 3.14 / 2.0f);
	beck.material.color = {rand_interval(), rand_interval(), rand_interval()};
	beck.material.phong.specular = 0.0f;

	bird_md.add(base, "base");
	bird_md.add(aile_g0, "aile_g0", "base");
	bird_md.add(aile_g1, "aile_g1", "aile_g0");
	bird_md.add(aile_d0, "aile_d0", "base");
	bird_md.add(aile_d1, "aile_d1", "aile_d0");
	bird_md.add(head, "head", "base");
	bird_md.add(beck, "beck", "head");
}