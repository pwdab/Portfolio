//
//  Tiger.h
//
//

#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include "../../Data.h"
#include "../../../src/Geometry.h"
#include "../../../src/Shader.h"

#define N_TIGER_FRAMES 12

#define KEY_FRAME_1 -30
#define KEY_FRAME_2 69
#define KEY_FRAME_3 360
#define KEY_FRAME_4 440

//Tiger.cpp
void prepare_my_tiger_20191576(void);
void draw_my_tiger_20191576(void);