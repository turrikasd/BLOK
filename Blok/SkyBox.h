#pragma once

#include <GL\glew.h>
#include "TextureLoader.h"
#include "assimp\scene.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Load();
	void Render(glm::mat4 VP);

private:
	GLuint texIDs[6];
	GLuint skyboxProgram;
	GLuint skyboxVPMatrixID;

	GLuint shaderIDs[6];
};

