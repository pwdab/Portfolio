//
//  Grid.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include "../src/Geometry.h"

#define GRID_LENGTH			(100)
#define NUM_GRID_VETICES	((2 * GRID_LENGTH + 1) * 4)

//Grid.cpp
void prepare_grid(void);
void draw_grid(void);