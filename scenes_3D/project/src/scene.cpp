#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	std::cout << "Start function scene_structure::initialize()" << std::endl;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window);
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{-196.0f, 226.0f, 15.0f} /* position of the camera in the 3D scene */,
		{-44.0f, 89.0f, 15.0f} /* targeted point in 3D scene */,
		{0, 0, 1} /* direction of the "up" vector */);
	//{-0.5f, 34.0f, 15.0f}
	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// shadings
	opengl_shader_structure shader_mesh;
	shader_mesh.load(
		project::path + "shaders/mesh/mesh.vert.glsl",
		project::path + "shaders/mesh/mesh.frag.glsl");

	opengl_shader_structure shader_single_color;
	shader_single_color.load(
		project::path + "shaders/single_color/single_color.vert.glsl",
		project::path + "shaders/single_color/single_color.frag.glsl");

	opengl_shader_structure shader_water;
	shader_water.load(
		project::path + "shaders/mesh_water/mesh_water.vert.glsl",
		project::path + "shaders/mesh_water/mesh_water.frag.glsl");

	opengl_shader_structure shader_illumination;
	shader_illumination.load(
		project::path + "shaders/illumination/illumination.mesh.vert.glsl",
		project::path + "shaders/illumination/illumination.mesh.frag.glsl");

	opengl_shader_structure shader_fishes;
	shader_fishes.load(
		project::path + "shaders/fish/fish.mesh.vert.glsl",
		project::path + "shaders/fish/fish.mesh.frag.glsl");

	opengl_shader_structure shader_grass;
	shader_grass.load(
		project::path + "shaders/mesh_grass/mesh_grass.vert.glsl",
		project::path + "shaders/mesh_grass/mesh_grass.frag.glsl");
	// Create the shapes seen in the 3D scene
	bool display = true;

	if (display)
	{
		cube_c.initialize_data_on_gpu(mesh_primitive_cube());
		cube_c.model.scaling = 3;

		create_object_water(water, fwater, fwater_trees, shader_water, shader_illumination, shader_fishes, bWater);

		// with windmill
		cgp::vec3 center_land0 = cgp::vec3{40.0f, 40.0f, 0.0f};
		float scalew = scale * 3;
		create_object_land0(windmill0, rock0, tree0, golden_egg_md, golden_egg.position, golden_egg.min_dist, grass, N_herbe, shader_illumination, shader_grass, center_land0, scalew, bLand0);

		// with liberty statue
		float scale_land1 = scale * 1.5f;
		cgp::vec3 center_land1 = cgp::vec3{150.0f, -70.0f, 0.0f} * scale_land1;
		create_object_land1(rock1, torch.position, torch.min_dist, center_land1, shader_illumination, scale_land1, bLand1);
		mesh ms = mesh_primitive_sphere(2);
		for_test.initialize_data_on_gpu(ms);
		for_test.model.translation = torch.position;

		// with house
		float scale_land2 = scale * 3.0f;
		cgp::vec3 center_land2 = cgp::vec3{-90.0f, -70.0f, 0.0f} * scale_land2;
		create_object_land2(rock2, house_model_0, trees_land2, painting.position, painting.min_dist, center_land2, shader_illumination, scale_land2, bLand2);

		sun.initialize_data_on_gpu(mesh_primitive_sphere(20.0f));
		sun.material.color = gui.light_color;
		sun.material.phong.specular = 0.7f;
		sun.material.phong.diffuse = 1.0f;
		// sun.shader = shader_illumination;

		float l = 0.1f;
		mesh quad_mesh = mesh_primitive_quadrangle({-l / 2.0f, 0, 0}, {l / 2.0f, 0, 0}, {l / 2.0f, 0, l}, {-l / 2.0f, 0, l});
		target.initialize_data_on_gpu(quad_mesh);
		target.texture.load_and_initialize_texture_2d_on_gpu("assets/target/target_model_0.png");
		target.shader = shader_mesh;

		cgp::vec3 center_land3 = cgp::vec3{-171, 84.0f, 0.0f};
		create_object_land3(rocket_platform, rocket, 10, center_land3, shader_illumination, scale, bLand3);

		for (size_t i = 0; i < N_birds; i++)
		{
			birds_structure bird;
			bird.initialize();
			birds.push_back(bird);
		}

		mesh japan_land_mesh = mesh_load_file_obj("assets/blender_obj/Land/japan_land/finaljapon.obj");
		japan_land.initialize_data_on_gpu(japan_land_mesh);
		japan_land.texture.load_and_initialize_texture_2d_on_gpu("assets/blender_obj/Land/japan_land/texture2.png", GL_REPEAT, GL_REPEAT);
		japan_land.model.scaling = 7;
		japan_land.model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2.0f);
		japan_land.model.translation = cgp::vec3{5.0f, 5.0f, -250.0f};
		japan_land.shader = shader_illumination;
		bJapanLand = true;

		for (size_t i = 0; i < N_fishes; i++)
		{
			fishes_structure fish;
			fish.initialize();
			fishes.push_back(fish);
		}
	}

	///
}

// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	// environment.light = camera_control.camera_model.position();
	environment.uniform_generic.uniform_float["ambiant"] = gui.ambiant;
	environment.uniform_generic.uniform_float["diffuse"] = gui.diffuse;
	environment.uniform_generic.uniform_float["specular"] = gui.specular;
	environment.uniform_generic.uniform_float["specular_exp"] = gui.specular_exp;
	environment.uniform_generic.uniform_vec3["light_color"] = gui.light_color;
	environment.background_color = gui.background_color;
	gui.light0_position = {-10.0f, 550 * std::sin(0.05 * timer.t), 550 * std::abs(std::cos(0.05 * timer.t))};
	environment.uniform_generic.uniform_vec3["light0_position"] = gui.light0_position;
	environment.uniform_generic.uniform_vec3["light1_position"] = camera_control.camera_model.position();

	environment.uniform_generic.uniform_float["time"] = timer.t;

	// Update time
	timer.update();

	// draw(cube_c, environment);

	// birds
	for (size_t i = 0; i < birds.size(); i++)
		birds[i].display(environment, timer.t);

	// fishes
	for (size_t i = 0; i < fishes.size(); i++)
		fishes[i].display(environment, timer.t);

	// goldenegg
	draw(golden_egg_md, environment);

	if (bLand0)
	{
		draw(grass, environment, N_herbe);
		windmill0["bolt2"].transform_local.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -timer.t);
		windmill0.update_local_to_global_coordinates();
		draw(windmill0, environment);
		draw(rock0, environment);
		draw(tree0, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(windmill0, environment);
			draw_wireframe(rock0, environment);
			draw_wireframe(tree0, environment);
		}
	}

	if (bLand1)
	{
		draw(rock1, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(rock1, environment);
		}
	}

	if (bLand2)
	{
		draw(house_model_0, environment);
		draw(trees_land2, environment);
		draw(rock2, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(rock2, environment);
			draw_wireframe(house_model_0, environment);
			draw_wireframe(trees_land2, environment);
		}
	}

	if (bLand3)
	{
		draw(rocket, environment);
		draw(rocket_platform, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(rocket, environment);
			draw_wireframe(rocket_platform, environment);
		}
	}

	if (bJapanLand)
	{
		draw(japan_land, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(japan_land, environment);
		}
	}

	if (bWater)
	{
		draw(fwater, environment);
		draw(fwater_trees, environment);
		draw(water, environment);
		if (gui.display_wireframe)
		{
			draw_wireframe(fwater, environment);
			draw_wireframe(fwater_trees, environment);
			draw_wireframe(water, environment);
		}
	}

	if (gui.light0_position.z > -40.0f)
	{
		sun.model.translation = gui.light0_position;
		draw(sun, environment);
	}

	if (gui.display_frame)
	{
		draw(global_frame, environment);
	}

	// target
	target.model.rotation = rotation_transform::from_frame_transform({1, 0, 0}, {0, 0, 1}, camera_control.camera_model.right(), camera_control.camera_model.up());
	target.model.translation = camera_control.camera_model.position_camera + camera_control.camera_model.front();
	draw(target, environment);
}

void scene_structure::display_gui()
{
	if (ImGui::CollapsingHeader("App Settings"))
	{
		ImGui::Checkbox("Frame", &gui.display_frame);
		ImGui::Checkbox("Wireframe", &gui.display_wireframe);
		ImGui::ColorEdit3("Sky color", &gui.background_color[0]);
		ImGui::ColorEdit3("Light color", &gui.light_color[0]);
		ImGui::SliderFloat("Ambiant", &gui.ambiant, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffus", &gui.diffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular", &gui.specular, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Exp", &gui.specular_exp, 0.1f, 256.0f);
	}

	if (ImGui::CollapsingHeader("Developers"))
	{
		ImGui::Text("Abasse DABERE");
		ImGui::Text("Oumouhanni EL-VILALY");
	}
}

void scene_structure::display_gui_objects()
{

	ImVec2 screenSize(window.width, window.height);
	ImVec2 windowSize(300, 400);
	ImVec2 windowPos(10, (screenSize.y - windowSize.y) / 1.5f);

	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	ImGui::Begin("HIDDEN OBJECTS", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowFontScale(1 / 5.0f);
	ImGui::Text("Find these hidden objects");
	if (painting.find)
		ImGui::TextDisabled("Painting");
	else
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Painting");

	if (torch.find)
		ImGui::TextDisabled("Torch");
	else
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Torch");
	if (golden_egg.find)
		ImGui::TextDisabled("Golden egg");
	else
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Golden egg");

	ImGui::End();
}

void scene_structure::display_gui_bravo()
{
	if (show_congrats)
	{
		ImVec2 screenSize(window.width, window.height);
		ImVec2 windowSize(600, 200);
		ImVec2 windowPos((screenSize.x - windowSize.x) / 2.0f, (screenSize.y - windowSize.y) / 2.0f);

		ImGui::SetNextWindowPos(windowPos);
		ImGui::SetNextWindowSize(windowSize);
		ImGui::Begin("Bravo Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		// Centrer le texte horizontalement
		float textWidth = ImGui::CalcTextSize("CONGRATS").x;
		float centerPosX = (windowSize.x - textWidth) / 2.0f;
		ImGui::SetCursorPosX(centerPosX);
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "CONGRATS");
		ImGui::End();
		fps_congrats++;
	}
	if (fps_congrats > 150)
	{
		show_congrats = false;
		fps_congrats = 0;
	}
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	if (camera_control.inputs->keyboard.last_action.is_pressed(GLFW_KEY_O))
	{
		std::cout << "camera position :" << camera_control.camera_model.position_camera << std::endl;
	}
	// camera_control.action_keyboard(environment.camera_view);
	if (camera_control.inputs->keyboard.last_action.is_pressed(GLFW_KEY_ENTER))
	{
		long count = 0;
		for (const auto &it : hidden_objects)
		{
			cgp::vec3 obj_camera = it->position - camera_control.camera_model.position_camera;

			float dist_to_camera = cgp::norm(obj_camera);
			float alpha = std::acos(cgp::dot(obj_camera, camera_control.camera_model.front()) / (dist_to_camera));
			if (dist_to_camera < it->min_dist && !it->find)
			{
				if (count == 0 && -1.0f < (it->position.x - camera_control.camera_model.position_camera.x) && alpha < 0.22f) // painting
				{
					it->find = true;
					show_congrats = true;
				}
				else if (alpha < 0.2f)
				{
					it->find = true;
					show_congrats = true;
				}
			}

			// std::cout << "hidden :" << count << std::endl;
			// std::cout << "alpha :" << alpha << std::endl;
			// std::cout << "dist_to_camera :" << dist_to_camera << std::endl;

			// std::cout << std::endl;
			count++;
		}
	}
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}
