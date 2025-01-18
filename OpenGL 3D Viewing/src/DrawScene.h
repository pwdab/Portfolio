//
//  DrawScene.h
//
//  Written for CSE4170
//  Department of Computer Science and Engineering
//  Copyright © 2023 Sogang University. All rights reserved.
//

#pragma once
#include "CallBack.h"
#include "Geometry.h"
#include "../Geometry/Axes.h"
#include "../Geometry/Grid.h"
#include "../Geometry/BistroExterior.h"
#include "../Geometry/Skybox.h"
#include "LoadScene.h"
#include "Camera.h"
#include "Shader.h"

// DrawScene.cpp
void register_callbacks(void);
void prepare_scene(void);
void initialize_renderer(void);
void initialize_glew(void);
void print_message(const char* m);
void greetings(char* program_name, char messages[][256], int n_message_lines);
void drawScene(int argc, char* argv[]);