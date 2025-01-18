//
//  Shader.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../Shaders/LoadShaders.h"
#include "ShadingInfo.h"

#define NUMBER_OF_LIGHT_SUPPORTED 1

#define TEXTURE_INDEX_DIFFUSE	(0)
#define TEXTURE_INDEX_NORMAL	(1)
#define TEXTURE_INDEX_SPECULAR	(2)
#define TEXTURE_INDEX_EMISSIVE	(3)
#define TEXTURE_INDEX_SKYMAP	(4)

//Shader.cpp
void prepare_shader_program(void);