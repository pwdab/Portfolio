#include "Tiger.h"

//Geometry.h
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
extern glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
extern glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.
extern glm::mat4 ModelViewMatrix;
extern glm::mat3 ModelViewMatrixInvTrans;

//Shader.h
extern GLuint h_ShaderProgram_simple;
extern GLint loc_ModelViewProjectionMatrix, loc_primitive_color;

// for tiger animation
unsigned int timestamp_scene = 0; // the global clock in the scene
int flag_tiger_animation = 1;
int timestamp_tiger = -30;
int cur_frame_tiger = 0;
float rotation_angle_tiger = 0.0f;

// tiger object
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat* tiger_vertices[N_TIGER_FRAMES];
GLfloat tiger_color[3] = { 1.0f, 0.0f, 0.0f };
float tiger_velocity = 750 * TO_RADIAN;

void prepare_my_tiger_20191576(void) { // vertices enumerated clockwise
	int i, n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/tiger/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		tiger_n_triangles[i] = read_geometry(&tiger_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		tiger_n_total_triangles += tiger_n_triangles[i];

		if (i == 0)
			tiger_vertex_offset[i] = 0;
		else
			tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &tiger_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_TIGER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, tiger_vertex_offset[i] * n_bytes_per_vertex,
			tiger_n_triangles[i] * n_bytes_per_triangle, tiger_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_TIGER_FRAMES; i++)
		free(tiger_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &tiger_VAO);
	glBindVertexArray(tiger_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_my_tiger_20191576(void) {
	glUseProgram(h_ShaderProgram_simple);

	if (KEY_FRAME_1 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_2) {
		ModelViewMatrix = glm::rotate(ViewMatrix, rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-750.0f, 0.0f, 0.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	}
	else if (KEY_FRAME_2 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_3) {
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(cos((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (timestamp_tiger - KEY_FRAME_2),
															   -sin((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (timestamp_tiger - KEY_FRAME_2),
															   0.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, KEY_FRAME_2 * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-750.0f, 0.0f, 0.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	}
	else if (KEY_FRAME_3 <= timestamp_tiger && timestamp_tiger < KEY_FRAME_4) {
		int radius = 600;
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(cos((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * cos(KEY_FRAME_2 * TO_RADIAN),
															   -sin((90 - KEY_FRAME_2) * TO_RADIAN) * tiger_velocity * (KEY_FRAME_3 - KEY_FRAME_2) - (2 * radius + (tiger_velocity / TO_RADIAN - radius)) * sin(KEY_FRAME_2 * TO_RADIAN),
															   0.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, KEY_FRAME_2 * TO_RADIAN - rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(radius, 0.0f, 0.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	}
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform3fv(loc_primitive_color, 1, tiger_color);

	glBindVertexArray(tiger_VAO);
	glDrawArrays(GL_TRIANGLES, tiger_vertex_offset[cur_frame_tiger], 3 * tiger_n_triangles[cur_frame_tiger]);
	glBindVertexArray(0);
	glUseProgram(0);
}