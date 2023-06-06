
#pragma once

#include "cgp/cgp.hpp"

#include "houses.hpp"
#include "windmill.hpp"
#include "rock.hpp"
#include "tree.hpp"
#include "custom_primitive.hpp"
#include "terrain.hpp"

void create_object_land0(cgp::hierarchy_mesh_drawable &windmill0,
                         cgp::mesh_drawable &rock0,
                         cgp::mesh_drawable &tree0,
                         cgp::mesh_drawable &tree2,
                         long const &N_herbe,
                         cgp::opengl_shader_structure const &shader_illumination,
                         cgp::opengl_shader_structure const &shader_grass,
                         cgp::vec3 const &center,
                         float const &scale,
                         bool &bLand0);

void create_object_land1(cgp::mesh_drawable &rock1,
                         cgp::vec3 &torch_position,
                         float &torch_min_dist,
                         cgp::vec3 &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float &scale,
                         bool &bLand1);

void create_object_land2(cgp::mesh_drawable &rock2,
                         cgp::hierarchy_mesh_drawable &house_model_0,
                         cgp::vec3 &painting_position,
                         float &painting_min_dist,
                         cgp::vec3 &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float &scale,
                         bool &bLand2);

void create_object_land3(cgp::mesh_drawable &rocket_platform,
                         cgp::mesh_drawable &rocket,
                         float height,
                         cgp::vec3 const &center,
                         cgp::opengl_shader_structure &shader_illumination,
                         float const &scale,
                         bool &bLand3);

void create_object_water(cgp::mesh_drawable &water,
                         cgp::mesh_drawable &fwater,
                         cgp::mesh_drawable &fishes,
                         long &N_fishes,
                         cgp::mesh_drawable &trees,
                         cgp::opengl_shader_structure &shader_water,
                         cgp::opengl_shader_structure &shader_illumination,
                         cgp::opengl_shader_structure &shader_fishes,
                         bool &bWater);
