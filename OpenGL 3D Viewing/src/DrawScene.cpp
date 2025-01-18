//
//  DrawScene.cpp
//
//  Written for CSE4170
//  Department of Computer Science and Engineering
//  Copyright © 2023 Sogang University. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include "DrawScene.h"

//Geometry.h
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
extern glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
extern glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.
extern glm::mat4 ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;
extern bool b_draw_grid;

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mousepress);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}

void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	initialize_lights();
}

void prepare_scene(void) {
	prepare_axes();
	prepare_grid();
	prepare_bistro_exterior();
	prepare_skybox();

	prepare_my_tiger_20191576();
	prepare_my_wolf_20191576();
	prepare_my_ben_20191576();

	prepare_my_bike_20191576();
	prepare_my_cow_20191576();
	prepare_my_godzilla_20191576();
	prepare_my_dragon_20191576();
	prepare_my_bus_20191576();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
	initialize_camera();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "********************************************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "********************************************************************************\n\n");
}

void print_message(const char* m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "********************************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n********************************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 26
void drawScene(int argc, char* argv[]) {
	char program_name[64] = "Sogang CSE4170 Bistro Exterior Scene";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Camera Selection:",
		"		'u' : set the static camera for original view",
		"		'i' : set the static camera for  view",
		"		'o' : set the static camera for  view",
		"		'p' : set the static camera for  view",
		"		'a' : set the dynamic camera",
		"		't' : set the static camera for tiger front view",
		"		'g' : set the static camera for tiger back view",
		"",
		"    - Camera Translation/Rotation:",
		"		'LEFT_KEY'  : translate camera left",
		"		'RIGHT_KEY' : translate camera right",
		"		'UP_KEY'    : translate camera back",
		"		'DOWN_KEY'  : translate camera forward",
		"		'z'         : translate camera up",
		"		'c'         : translate camera down",
		"		'Shift + LEFT_KEY'  : camera yaw rotate to left",
		"		'Shift + RIGHT_KEY' : camera yaw rotate to right",
		"		'Shift + UP_KEY'    : camera pitch rotate to up",
		"		'Shift + DOWN_KEY'  : camera pitch rotate to down",
		"		'Shift + z'         : camera roll rotate to left",
		"		'Shift + c'         : camera roll rotate to right",
		"",
		"    - Extra Keys used:",
		"		'f' : draw x, y, z axes and grid",
		"		'ESC' : program close"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutInitWindowPosition(20, 20);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}