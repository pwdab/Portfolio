//
//  Data.h
//
//

#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

//Data.cpp
int read_geometry(GLfloat** object, int bytes_per_primitive, char* filename);