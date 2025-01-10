//
//  CallBack.h
//
//
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "Geometry.h"
#include "../Geometry/Axes.h"
#include "../Geometry/Grid.h"
#include "../Geometry/BistroExterior.h"
#include "../Geometry/Skybox.h"
#include "LoadScene.h"
#include "Camera.h"
#include "../Data/Data.h"
#include "../Data/dynamic_objects/tiger/Tiger.h"
#include "../Data/dynamic_objects/wolf/Wolf.h"
#include "../Data/dynamic_objects/ben/Ben.h"
#include "../Data/static_objects/bike/Bike.h"
#include "../Data/static_objects/cow/Cow.h"
#include "../Data/static_objects/godzilla/Godzilla.h"
#include "../Data/static_objects/dragon/Dragon.h"
#include "../Data/static_objects/bus/Bus.h"


#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

// DrawScene.cpp
void display(void);
void timer_scene(int value);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void mousepress(int button, int state, int x, int y);
void reshape(int width, int height);
void cleanup(void);
