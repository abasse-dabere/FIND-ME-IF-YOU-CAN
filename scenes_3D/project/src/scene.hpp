#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "custom_camera_controller.hpp"

#include "houses.hpp"
#include "create_objects.hpp"

#include "birds_structure.hpp"
#include "fishes_structure.hpp"

#include <string>

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

// Variables associated to the GUI (buttons, etc)
struct gui_parameters
{
	bool display_frame = false;
	bool display_wireframe = false;

	vec3 light_color = {1, 1, 1};
	vec3 background_color = {0, 102 / 256.0f, 204 / 256.0f};
	vec3 light0_position = {-2, 2, 2};
	vec3 light1_position = {-2, 2, 2};
	float ambiant = 0.6f;
	float diffuse = 1.0f;
	float specular = 0.0f;
	float specular_exp = 0.1f;
};

struct hidden_object
{
	cgp::vec3 position;
	float min_dist = 1000.0f;
	bool find = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic
{
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	// camera_controller_orbit_euler camera_control;
	// camera_controller_2d_displacement camera_control;
	// camera_controller_fly_mode camera_control;
	// camera_controller_first_person camera_control;
	// camera_controller_generic_base camera_control;
	custom_camera_controller camera_control;

	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;		   // The standard global frame
	environment_structure environment; // Standard environment controler
	input_devices inputs;			   // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;				   // Standard GUI element storage

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	mesh_drawable for_test;

	mesh_drawable cube_c;
	mesh_drawable sun;

	timer_basic timer;
	// float scale = 0.32f;
	float scale = 0.32f;
	float l_off = 1;

	bool bWater = false;
	mesh_drawable water;
	mesh_drawable fwater;
	mesh_drawable deco_rock0;

	bool bLand0 = false;
	long N_herbe = 1000;
	cgp::hierarchy_mesh_drawable windmill0;
	cgp::mesh_drawable rock0;
	mesh_drawable tree0;
	mesh_drawable tree1;
	mesh_drawable tree_model_1;
	mesh_drawable tree2;
	mesh_drawable tree3;
	mesh_drawable grass;

	mesh_drawable mtree;

	bool bLand1 = false;
	cgp::hierarchy_mesh_drawable house0;
	mesh_drawable rock1;

	bool bLand2 = false;
	mesh_drawable rock2;
	// bool bDecoRock0 = false;

	mesh_drawable rocket_platform;
	mesh_drawable rocket;
	bool bLand3 = false;

	mesh_drawable japan_land;
	bool bJapanLand = false;

	mesh_drawable libertystatue;
	hidden_object torch;

	cgp::hierarchy_mesh_drawable house_model_0;
	cgp::mesh_drawable trees_land2;
	hidden_object painting;

	mesh_drawable target;

	mesh_drawable golden_egg_md;
	hidden_object golden_egg;

	std::vector<hidden_object *> hidden_objects = {&painting, &torch, &golden_egg};
	bool show_congrats = false;
	long fps_congrats = 0;

	long N_birds = 5;
	long N_fishes = 5;
	std::vector<birds_structure> birds;
	std::vector<fishes_structure> fishes;

	mesh_drawable fwater_trees;
	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();	  // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();	  // The display of the GUI, also called within the animation loop

	void display_gui_objects();
	void display_gui_bravo();
	std::string fname = "TEST";

	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
};
