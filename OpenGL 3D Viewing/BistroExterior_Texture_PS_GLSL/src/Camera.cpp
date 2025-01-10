#include "Camera.h"

//Geometry.h
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
extern glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
extern glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.
extern glm::mat4 ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;
extern bool b_draw_grid;

//LoadScene.h
extern SCENE scene;

Camera camera_info[NUM_CAMERAS];
Camera current_camera;

using glm::mat4;
void set_ViewMatrix_from_camera_frame(void) {
	ViewMatrix = glm::mat4(current_camera.uaxis[0], current_camera.vaxis[0], current_camera.naxis[0], 0.0f,
		current_camera.uaxis[1], current_camera.vaxis[1], current_camera.naxis[1], 0.0f,
		current_camera.uaxis[2], current_camera.vaxis[2], current_camera.naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-current_camera.pos[0], -current_camera.pos[1], -current_camera.pos[2]));
}

void set_current_camera(int camera_num) {
	Camera* pCamera = &camera_info[camera_num];

	memcpy(&current_camera, pCamera, sizeof(Camera));
	set_ViewMatrix_from_camera_frame();
	ProjectionMatrix = glm::perspective(current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void initialize_camera(void) {
	Camera* pCamera;

	//bistro left view
	//CAMERA_U : Bistro Static Camera 1 (Original View)
	pCamera = &camera_info[CAMERA_U];
	pCamera->pos[0] = -1715.251709f; pCamera->pos[1] = 249.633179f; pCamera->pos[2] = 486.491394f;
	pCamera->uaxis[0] = -0.266408f; pCamera->uaxis[1] = -0.963670f; pCamera->uaxis[2] = 0.019177f;
	pCamera->vaxis[0] = 0.205614f; pCamera->vaxis[1] = -0.037383f; pCamera->vaxis[2] = 0.977912f;
	pCamera->naxis[0] = -0.941668f; pCamera->naxis[1] = 0.264468f; pCamera->naxis[2] = 0.208104f;
	pCamera->move = 0;
	pCamera->fovy = 46 * TO_RADIAN, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;


	//bistro right view
	//CAMERA_I : Bistro Static Camera 2
	pCamera = &camera_info[CAMERA_I];
	pCamera->pos[0] = -957.383179f; pCamera->pos[1] = -637.313110f; pCamera->pos[2] = 497.274750f;
	pCamera->uaxis[0] = 0.930188f; pCamera->uaxis[1] = -0.366867f; pCamera->uaxis[2] = 0.012840f;
	pCamera->vaxis[0] = 0.052802f; pCamera->vaxis[1] = 0.168325f; pCamera->vaxis[2] = 0.984310f;
	pCamera->naxis[0] = -0.363271f; pCamera->naxis[1] = -0.914915f; pCamera->naxis[2] = 0.175946f;
	pCamera->move = 0;
	pCamera->fovy = 56 * TO_RADIAN, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//bistro tree view
	//CAMERA_O : Bistro Static Camera 3
	pCamera = &camera_info[CAMERA_O];
	pCamera->pos[0] = 5479.732422f; pCamera->pos[1] = -3478.593994f; pCamera->pos[2] = 252.724106f;
	pCamera->uaxis[0] = 0.619838f; pCamera->uaxis[1] = 0.784683f; pCamera->uaxis[2] = 0.008956f;
	pCamera->vaxis[0] = -0.014499f; pCamera->vaxis[1] = 0.000043f; pCamera->vaxis[2] = 0.999889f;
	pCamera->naxis[0] = 0.784595f; pCamera->naxis[1] = -0.619897f; pCamera->naxis[2] = 0.011405f;
	pCamera->move = 0;
	pCamera->fovy = 60 * TO_RADIAN, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//bistro street view
	//CAMERA_P : Bistro Static Camera 4
	pCamera = &camera_info[CAMERA_P];
	pCamera->pos[0] = 1872.159668f; pCamera->pos[1] = 4062.232422f; pCamera->pos[2] = 522.050049f;
	pCamera->uaxis[0] = -0.846022f; pCamera->uaxis[1] = 0.533142f; pCamera->uaxis[2] = 0.003307f;
	pCamera->vaxis[0] = -0.096664f; pCamera->vaxis[1] = -0.159486f; pCamera->vaxis[2] = 0.982459f;
	pCamera->naxis[0] = 0.524317f; pCamera->naxis[1] = 0.830860f; pCamera->naxis[2] = 0.186464f;
	pCamera->move = 0;
	pCamera->fovy = 46 * TO_RADIAN, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//top view
	//CAMERA_A : Bistro Dynamic Camera
	pCamera = &camera_info[CAMERA_A];
	
	
	pCamera->pos[0] = -4210.0f; pCamera->pos[1] = -95.0f; pCamera->pos[2] = 9150.0f;
	pCamera->uaxis[0] = 0.0f; pCamera->uaxis[1] = -1.00f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.85f; pCamera->vaxis[1] = 0.0f; pCamera->vaxis[2] = 0.51f;
	pCamera->naxis[0] = -0.51f; pCamera->naxis[1] = 0.0f; pCamera->naxis[2] = 0.85f;
	
	/*
	pCamera->pos[0] = 950.0f; pCamera->pos[1] = -100.0f; pCamera->pos[2] = 12800.0f;
	pCamera->uaxis[0] = 1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 1.0f; pCamera->vaxis[2] = 0.0f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 0.0f; pCamera->naxis[2] = 1.0f;
	*/
	
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//tiger first-person view
	//CAMERA_T : Tiger Static Camera 1
	pCamera = &camera_info[CAMERA_T];
	pCamera->pos[0] = 0.0f * 2.0f - 750.0f; pCamera->pos[1] = -88.0f * 2.0f; pCamera->pos[2] = 62.0f * 2.0f;
	pCamera->uaxis[0] = -1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 0.0f; pCamera->vaxis[2] = 1.0f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 1.0f; pCamera->naxis[2] = 0.0f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//tiger third-person view
	//CAMERA_G : Tiger Static Camera 2
	pCamera = &camera_info[CAMERA_G];
	pCamera->pos[0] = 0.0f * 2.0f - 750.0f; pCamera->pos[1] = 350.0f * 2.0f; pCamera->pos[2] = 130.0f * 2.0f;
	pCamera->uaxis[0] = -1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = -0.121869f; pCamera->vaxis[2] = 0.992546f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 0.992546f; pCamera->naxis[2] = 0.121869f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	set_current_camera(CAMERA_U);
}