#include "CallBack.h"

//Geometry.h
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
extern glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
extern glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.
extern glm::mat4 ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;
extern bool b_draw_grid;

//Geometry/Axes.h
extern GLuint axes_VBO, axes_VAO;
//Geometry/Grid.h
extern GLuint grid_VBO, grid_VAO;
//Geometry/BistroExterior.h
extern GLuint* bistro_exterior_VBO;
extern GLuint* bistro_exterior_VAO;
extern int* bistro_exterior_n_triangles;
extern int* bistro_exterior_vertex_offset;
extern GLuint* bistro_exterior_texture_names;
extern bool* flag_texture_mapping;
//Geometry/Skybox.h
extern GLuint skybox_VBO, skybox_VAO;

//LoadScene.h
extern SCENE scene;

//Camera.h
extern Camera camera_info[];
extern Camera current_camera;

//Shader.h
extern GLuint h_ShaderProgram_simple;
extern GLint loc_ModelViewProjectionMatrix, loc_primitive_color;

//Data.h
//extern GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;

//Tiger.h
extern unsigned int timestamp_scene;
extern int flag_tiger_animation;
extern int timestamp_tiger;
extern int cur_frame_tiger;
extern float rotation_angle_tiger;
extern float tiger_velocity;

//Wolf.h
extern int timestamp_wolf;
extern int cur_frame_wolf;
extern float rotation_angle_wolf;

//Ben.h
extern int timestamp_ben;
extern int cur_frame_ben;
extern float rotation_angle_ben;

int current_camera_type = CAMERA_U;

void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_grid();
	draw_axes();
	draw_bistro_exterior();
	draw_skybox();

	draw_my_tiger_20191576();
	draw_my_wolf_20191576();
	draw_my_ben_20191576();

	draw_my_bike_20191576();
	draw_my_cow_20191576();
	draw_my_godzilla_20191576();
	draw_my_dragon_20191576();
	draw_my_bus_20191576();

	glutSwapBuffers();
}

glm::mat4 prev_camera_t;
glm::mat4 prev_camera_g;
void timer_scene(int value) {
	timestamp_scene = (timestamp_scene + 1) % UINT_MAX;
	if (flag_tiger_animation) {
		timestamp_tiger = (timestamp_tiger - KEY_FRAME_1 + 1) % KEY_FRAME_4 + KEY_FRAME_1;
		cur_frame_tiger = timestamp_scene % N_TIGER_FRAMES;
	}
	timestamp_wolf = (timestamp_wolf + 1) % 250;
	cur_frame_wolf = timestamp_scene % N_WOLF_FRAMES;
	rotation_angle_wolf = rotation_angle_ben = (timestamp_scene % 360) * TO_RADIAN;

	timestamp_ben = (timestamp_ben + 1) % 360;
	cur_frame_ben = (timestamp_scene * 2) % N_BEN_FRAMES;
	
	// Tiger Animation
	// CAMERA_T
	glm::mat4 camera_t;
	camera_t = glm::mat4(camera_info[CAMERA_T].pos[0], camera_info[CAMERA_T].uaxis[0], camera_info[CAMERA_T].vaxis[0], camera_info[CAMERA_T].naxis[0],
		camera_info[CAMERA_T].pos[1], camera_info[CAMERA_T].uaxis[1], camera_info[CAMERA_T].vaxis[1], camera_info[CAMERA_T].naxis[1],
		camera_info[CAMERA_T].pos[2], camera_info[CAMERA_T].uaxis[2], camera_info[CAMERA_T].vaxis[2], camera_info[CAMERA_T].naxis[2],
		0.0f, 0.0f, 0.0f, 1.0f);

	// CAMERA_G
	glm::mat4 camera_g;
	camera_g = glm::mat4(camera_info[CAMERA_G].pos[0], camera_info[CAMERA_G].uaxis[0], camera_info[CAMERA_G].vaxis[0], camera_info[CAMERA_G].naxis[0],
		camera_info[CAMERA_G].pos[1], camera_info[CAMERA_G].uaxis[1], camera_info[CAMERA_G].vaxis[1], camera_info[CAMERA_G].naxis[1],
		camera_info[CAMERA_G].pos[2], camera_info[CAMERA_G].uaxis[2], camera_info[CAMERA_G].vaxis[2], camera_info[CAMERA_G].naxis[2],
		0.0f, 0.0f, 0.0f, 1.0f);

	if (-30 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_2) {
		rotation_angle_tiger = (timestamp_tiger % KEY_FRAME_2) * TO_RADIAN;
		//Rotation
		camera_t = glm::rotate(camera_t, -rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		camera_g = glm::rotate(camera_g, -rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (KEY_FRAME_2 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_3) {
		//Rotation
		camera_t = glm::rotate(camera_t, -(KEY_FRAME_2 * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
		camera_g = glm::rotate(camera_g, -(KEY_FRAME_2 * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));

		float naxis = sqrt(pow(camera_t[0][3], 2) + pow(camera_t[1][3], 2) + pow(camera_t[2][3], 2));
		//Translation
		camera_t[0][0] -= camera_t[0][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
		camera_t[1][0] -= camera_t[1][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
		camera_t[2][0] -= camera_t[2][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
		camera_g[0][0] -= camera_t[0][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
		camera_g[1][0] -= camera_t[1][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
		camera_g[2][0] -= camera_t[2][3] / naxis * tiger_velocity * (timestamp_tiger - KEY_FRAME_2);
	}
	else if (KEY_FRAME_3 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_4) {
		/*
		camera_t = glm::rotate(camera_t, -(KEY_FRAME_2 * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
		camera_g = glm::rotate(camera_g, -(KEY_FRAME_2 * TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));

		float naxis = sqrt(pow(camera_t[0][3], 2) + pow(camera_t[1][3], 2) + pow(camera_t[2][3], 2));
		camera_t[0][0] -= camera_t[0][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);
		camera_t[1][0] -= camera_t[1][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);
		camera_t[2][0] -= camera_t[2][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);
		camera_g[0][0] -= camera_t[0][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);
		camera_g[1][0] -= camera_t[1][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);
		camera_g[2][0] -= camera_t[2][3] / naxis * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2);

		int radius = 600;
		rotation_angle_tiger = (timestamp_tiger % KEY_FRAME_3) * TO_RADIAN;
		int camera_t_x = camera_t[0][0] - radius * cos(KEY_FRAME_2 * TO_RADIAN), camera_t_y = camera_t[1][0] + radius * sin(KEY_FRAME_2 * TO_RADIAN);
		int camera_g_x = camera_g[0][0] - radius * cos(KEY_FRAME_2 * TO_RADIAN), camera_g_y = camera_g[1][0] + radius * sin(KEY_FRAME_2 * TO_RADIAN);
		camera_t[0][0] = radius * cos(KEY_FRAME_2 * TO_RADIAN);
		camera_t[1][0] = radius * sin(KEY_FRAME_2 * TO_RADIAN);
		camera_g[0][0] = radius * cos(KEY_FRAME_2 * TO_RADIAN);
		camera_g[1][0] = radius * sin(KEY_FRAME_2 * TO_RADIAN);
		camera_t = glm::rotate(camera_t, rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		camera_g = glm::rotate(camera_g, rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		camera_t[0][0] += camera_t_x;
		camera_t[1][0] += camera_t_y;
		camera_g[0][0] += camera_g_x;
		camera_g[1][0] += camera_g_y;
		*/

		int radius = 600;
		rotation_angle_tiger = (timestamp_tiger % KEY_FRAME_3) * TO_RADIAN;
		//Translation
		camera_t[0][0] += tiger_velocity / TO_RADIAN + radius;
		camera_g[0][0] += tiger_velocity / TO_RADIAN + radius;
		//Rotation
		camera_t = glm::rotate(camera_t, -KEY_FRAME_2 * TO_RADIAN + rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		camera_g = glm::rotate(camera_g, -KEY_FRAME_2 * TO_RADIAN + rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		//Translation
		camera_t[0][0] += cos((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * cos(KEY_FRAME_2 * TO_RADIAN);
		camera_t[1][0] += -sin((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * sin(KEY_FRAME_2 * TO_RADIAN);
		camera_g[0][0] += cos((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * cos(KEY_FRAME_2 * TO_RADIAN);
		camera_g[1][0] += -sin((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * sin(KEY_FRAME_2 * TO_RADIAN);
	}

	if (current_camera_type == CAMERA_T) {
		current_camera.pos[0] = camera_t[0][0];
		current_camera.pos[1] = camera_t[1][0];
		current_camera.pos[2] = camera_t[2][0];
		current_camera.uaxis[0] = camera_t[0][1];
		current_camera.uaxis[1] = camera_t[1][1];
		current_camera.uaxis[2] = camera_t[2][1];
		current_camera.vaxis[0] = camera_t[0][2];
		current_camera.vaxis[1] = camera_t[1][2];
		current_camera.vaxis[2] = camera_t[2][2];
		current_camera.naxis[0] = camera_t[0][3];
		current_camera.naxis[1] = camera_t[1][3];
		current_camera.naxis[2] = camera_t[2][3];
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
	else if (current_camera_type == CAMERA_G) {
		current_camera.pos[0] = camera_g[0][0];
		current_camera.pos[1] = camera_g[1][0];
		current_camera.pos[2] = camera_g[2][0];
		current_camera.uaxis[0] = camera_g[0][1];
		current_camera.uaxis[1] = camera_g[1][1];
		current_camera.uaxis[2] = camera_g[2][1];
		current_camera.vaxis[0] = camera_g[0][2];
		current_camera.vaxis[1] = camera_g[1][2];
		current_camera.vaxis[2] = camera_g[2][2];
		current_camera.naxis[0] = camera_g[0][3];
		current_camera.naxis[1] = camera_g[1][3];
		current_camera.naxis[2] = camera_g[2][3];
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
	
	glutPostRedisplay();
	glutTimerFunc(50, timer_scene, 0);
}

void keyboard(unsigned char key, int x, int y) {
	//Camera Rotation
	if (current_camera.move == 1) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			glm::mat4 current_camera_axis;
			glm::mat4 result;
			current_camera_axis = glm::mat4(current_camera.uaxis[0], current_camera.vaxis[0], current_camera.naxis[0], 0.0f,
											current_camera.uaxis[1], current_camera.vaxis[1], current_camera.naxis[1], 0.0f,
											current_camera.uaxis[2], current_camera.vaxis[2], current_camera.naxis[2], 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f);
			switch (key) {
			//Camera Rotate Up via Camera's n-Axis
			case 'z':
			case 'Z':
				result = glm::rotate(current_camera_axis, TO_RADIAN, glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]));
				current_camera.uaxis[0] = result[0][0];
				current_camera.uaxis[1] = result[1][0];
				current_camera.uaxis[2] = result[2][0];
				current_camera.vaxis[0] = result[0][1];
				current_camera.vaxis[1] = result[1][1];
				current_camera.vaxis[2] = result[2][1];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
				glutPostRedisplay();
				break;

			//Camera Rotate Down via Camera's n-Axis
			case 'c':
			case 'C':
				result = glm::rotate(current_camera_axis, -TO_RADIAN, glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]));
				current_camera.uaxis[0] = result[0][0];
				current_camera.uaxis[1] = result[1][0];
				current_camera.uaxis[2] = result[2][0];
				current_camera.vaxis[0] = result[0][1];
				current_camera.vaxis[1] = result[1][1];
				current_camera.vaxis[2] = result[2][1];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
				glutPostRedisplay();
				break;
			}
		}
	}

	switch (key) {
	/*
	//For DEBUG!!
	case '1':
		printf("================ Print Camera Perspectives ================\n\n");
		printf("fovy = %f, asepct_ratio = %f, move = %d\n\n", current_camera.fovy / TO_RADIAN, current_camera.aspect_ratio, current_camera.move);
		printf("(pos_x, pos_y, pos_z) = (%f, %f, %f)\n", current_camera.pos[0], current_camera.pos[1], current_camera.pos[2]);
		printf("uaxis = (%f, %f, %f)\n", current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
		printf("vaxis = (%f, %f, %f)\n", current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]);
		printf("naxis = (%f, %f, %f)\n", current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
		printf("\n");
		//printf("timestamp_tiger = %d\n", timestamp_tiger);
		//printf("rotation_angle_tiger = %f\n", rotation_angle_tiger / TO_RADIAN);
		//printf("\n");
		break;
	*/
	

	//Tiger Movement ON/OFF
	case 'r':
	case 'R':
		flag_tiger_animation = flag_tiger_animation ? false : true;
		break;

	/*
	//Grid ON/OFF
	case 'f':
	case 'F':
		b_draw_grid = b_draw_grid ? false : true;
		glutPostRedisplay();
		break;
	*/

	//CAMERA_U : Static Camera 1 (Original View)
	case 'u':
	case 'U':
		set_current_camera(CAMERA_U);
		current_camera_type = CAMERA_U;
		glutPostRedisplay();
		break;

	//CAMERA_I : Static Camera 2
	case 'i':
	case 'I':
		set_current_camera(CAMERA_I);
		current_camera_type = CAMERA_I;
		glutPostRedisplay();	
		break;

	//CAMERA_O : Static Camera 3
	case 'o':
	case 'O':
		set_current_camera(CAMERA_O);
		current_camera_type = CAMERA_O;
		glutPostRedisplay();
		break;

	//CAMERA_P : Static Camera 4
	case 'p':
	case 'P':
		set_current_camera(CAMERA_P);
		current_camera_type = CAMERA_P;
		glutPostRedisplay();
		break;

	//CAMERA_A : Dynamic Camera
	case 'a':
	case 'A':
		set_current_camera(CAMERA_A);
		current_camera_type = CAMERA_A;
		current_camera.move = 1;
		break;

	//CAMERA_T : Tiger First-Person Camera
	case 't':
	case 'T':
		set_current_camera(CAMERA_T);
		current_camera_type = CAMERA_T;
		glutPostRedisplay();
		break;

	//CAMERA_G : Tiger Third-Person Camra
	case 'g':
	case 'G':
		set_current_camera(CAMERA_G);
		current_camera_type = CAMERA_G;
		glutPostRedisplay();
		break;
		
	//Camera Move Up via Camera's v-Axis
	case 'z':
	case 'Z':
		if (current_camera.move == 1) {
			current_camera.pos[0] += 50 * current_camera.vaxis[0] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			current_camera.pos[1] += 50 * current_camera.vaxis[1] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			current_camera.pos[2] += 50 * current_camera.vaxis[2] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
		}
		break;
		
	//Camera Move Down via Camera's v-Axis
	case 'c':
	case 'C':
		if (current_camera.move == 1) {
			current_camera.pos[0] -= 50 * current_camera.vaxis[0] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			current_camera.pos[1] -= 50 * current_camera.vaxis[1] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			current_camera.pos[2] -= 50 * current_camera.vaxis[2] / sqrt(pow(current_camera.vaxis[0], 2) + pow(current_camera.vaxis[1], 2) + pow(current_camera.vaxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
		}
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void special(int key, int x, int y) {
	if (current_camera.move == 1) {
		//Camera Rotation
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			glm::mat4 current_camera_axis;
			glm::mat4 result;
			current_camera_axis = glm::mat4(current_camera.uaxis[0], current_camera.vaxis[0], current_camera.naxis[0], 0.0f,
											current_camera.uaxis[1], current_camera.vaxis[1], current_camera.naxis[1], 0.0f,
											current_camera.uaxis[2], current_camera.vaxis[2], current_camera.naxis[2], 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f);
			switch (key) {
			//Camera Rotate Left around Camera's v-Axis
			case GLUT_KEY_LEFT:
				result = glm::rotate(current_camera_axis, TO_RADIAN, glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]));
				current_camera.uaxis[0] = result[0][0];
				current_camera.uaxis[1] = result[1][0];
				current_camera.uaxis[2] = result[2][0];
				current_camera.naxis[0] = result[0][2];
				current_camera.naxis[1] = result[1][2];
				current_camera.naxis[2] = result[2][2];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;	
				glutPostRedisplay();
				break;

			//Camera Rotate Right around Camera's v-Axis
			case GLUT_KEY_RIGHT:
				result = glm::rotate(current_camera_axis, -TO_RADIAN, glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]));
				current_camera.uaxis[0] = result[0][0];
				current_camera.uaxis[1] = result[1][0];
				current_camera.uaxis[2] = result[2][0];
				current_camera.naxis[0] = result[0][2];
				current_camera.naxis[1] = result[1][2];
				current_camera.naxis[2] = result[2][2];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
				glutPostRedisplay();
				break;

			//Camera Move Back via Camera's u-Axis
			case GLUT_KEY_DOWN:
				result = glm::rotate(current_camera_axis, -TO_RADIAN, glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]));
				current_camera.vaxis[0] = result[0][1];
				current_camera.vaxis[1] = result[1][1];
				current_camera.vaxis[2] = result[2][1];
				current_camera.naxis[0] = result[0][2];
				current_camera.naxis[1] = result[1][2];
				current_camera.naxis[2] = result[2][2];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
				glutPostRedisplay();
				break;

			//Camera Move Front via Camera's u-Axis
			case GLUT_KEY_UP:
				result = glm::rotate(current_camera_axis, TO_RADIAN, glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]));
				current_camera.vaxis[0] = result[0][1];
				current_camera.vaxis[1] = result[1][1];
				current_camera.vaxis[2] = result[2][1];
				current_camera.naxis[0] = result[0][2];
				current_camera.naxis[1] = result[1][2];
				current_camera.naxis[2] = result[2][2];
				set_ViewMatrix_from_camera_frame();
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
				glutPostRedisplay();
				break;
			}
		}

		//Camera Translation
		switch (key) {
			//Camera Move Left via Camera's u-Axis
		case GLUT_KEY_LEFT:
			current_camera.pos[0] -= 50 * current_camera.uaxis[0] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			current_camera.pos[1] -= 50 * current_camera.uaxis[1] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			current_camera.pos[2] -= 50 * current_camera.uaxis[2] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

			//Camera Move Right via Camera's u-Axis
		case GLUT_KEY_RIGHT:
			current_camera.pos[0] += 50 * current_camera.uaxis[0] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			current_camera.pos[1] += 50 * current_camera.uaxis[1] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			current_camera.pos[2] += 50 * current_camera.uaxis[2] / sqrt(pow(current_camera.uaxis[0], 2) + pow(current_camera.uaxis[1], 2) + pow(current_camera.uaxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

			//Camera Move Back via Camera's n-Axis
		case GLUT_KEY_DOWN:
			current_camera.pos[0] += 50 * current_camera.naxis[0] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			current_camera.pos[1] += 50 * current_camera.naxis[1] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			current_camera.pos[2] += 50 * current_camera.naxis[2] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

			//Camera Move Front via Camera's n-Axis
		case GLUT_KEY_UP:
			current_camera.pos[0] -= 50 * current_camera.naxis[0] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			current_camera.pos[1] -= 50 * current_camera.naxis[1] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			current_camera.pos[2] -= 50 * current_camera.naxis[2] / sqrt(pow(current_camera.naxis[0], 2) + pow(current_camera.naxis[1], 2) + pow(current_camera.naxis[2], 2));
			set_ViewMatrix_from_camera_frame();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		}
	}
}

void mousepress(int button, int state, int x, int y) {
	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		switch (button) {
			//Camera Zoom-In
		case GLUT_WHEEL_UP:
			current_camera.fovy -= 1 * TO_RADIAN;
			if (current_camera.fovy < MIN_FOV * TO_RADIAN) {
				current_camera.fovy = MIN_FOV * TO_RADIAN;
			}
			set_ViewMatrix_from_camera_frame();
			ProjectionMatrix = glm::perspective(current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

			//Camera Zoom-Out
		case GLUT_WHEEL_DOWN:
			current_camera.fovy += 1 * TO_RADIAN;
			if (current_camera.fovy > MAX_FOV * TO_RADIAN) {
				current_camera.fovy = MAX_FOV * TO_RADIAN;
			}
			set_ViewMatrix_from_camera_frame();
			ProjectionMatrix = glm::perspective(current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		}
	}
}

void reshape(int width, int height) {
	float aspect_ratio;

	glViewport(0, 0, width, height);

	ProjectionMatrix = glm::perspective(current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &grid_VAO);
	glDeleteBuffers(1, &grid_VBO);

	glDeleteVertexArrays(scene.n_materials, bistro_exterior_VAO);
	glDeleteBuffers(scene.n_materials, bistro_exterior_VBO);
	glDeleteTextures(scene.n_textures, bistro_exterior_texture_names);

	glDeleteVertexArrays(1, &skybox_VAO);
	glDeleteBuffers(1, &skybox_VBO);

	free(bistro_exterior_n_triangles);
	free(bistro_exterior_vertex_offset);

	free(bistro_exterior_VAO);
	free(bistro_exterior_VBO);

	free(bistro_exterior_texture_names);
	free(flag_texture_mapping);
}