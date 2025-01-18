//
//  Camera.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "Geometry.h"
#include "LoadScene.h"

#define MAX_FOV 120
#define MIN_FOV 10

typedef enum {
	CAMERA_U,
	CAMERA_I,
	CAMERA_O,
	CAMERA_P,
	CAMERA_A,
	CAMERA_T,
	CAMERA_G,
	NUM_CAMERAS
} CAMERA_INDEX;

typedef struct _Camera {
	float pos[3];
	float uaxis[3], vaxis[3], naxis[3];
	float fovy, aspect_ratio, near_c, far_c;
	int move, rotation_axis;
} Camera;

void set_ViewMatrix_from_camera_frame(void);
void set_current_camera(int camera_num);
void initialize_camera(void);