//
//  BistroExterior.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include "../src/Geometry.h"
#include "../src/LoadScene.h"
#include "../src/Camera.h"
#include "../src/Shader.h"
#include "../src/ShadingInfo.h"

//BistroExterior.cpp
void initialize_lights(void);
bool readTexImage2D_from_file(char* filename);
void prepare_bistro_exterior(void);
void bindTexture(GLuint tex, int glTextureId, int texId);
void draw_bistro_exterior(void);