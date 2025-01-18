//
//  Skybox.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include "../src/Geometry.h"
#include "../src/LoadScene.h"
#include "../src/Shader.h"

//Skybox.cpp
void readTexImage2DForCubeMap(const char* filename, GLenum texture_target);
void prepare_skybox(void);
void draw_skybox(void);