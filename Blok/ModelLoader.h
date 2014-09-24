#pragma once

#include "GL\glew.h"
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"


void LoadModelElem(const char *file, GLuint* vertexBuffer, GLuint* UVBuffer, GLuint* normalBuffer, GLuint* elementBuffer, GLuint* numTriangles);