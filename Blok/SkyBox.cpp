#include "SkyBox.h"
#include "shader.hpp"

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::Load()
{
	texIDs[0] = LoadTexture("skycube\\sky01.png");
	texIDs[1] = LoadTexture("skycube\\sky04.png");
	texIDs[2] = LoadTexture("skycube\\sky05.png");
	texIDs[3] = LoadTexture("skycube\\sky02.png");
	texIDs[4] = LoadTexture("skycube\\sky03.png");
	texIDs[6] = LoadTexture("skycube\\sky06.png");

	skyboxProgram = LoadShaders("skybox.vert", "skybox.frag");
	skyboxVPMatrixID = glGetUniformLocation(skyboxProgram, "VP");

	shaderIDs[0] = glGetUniformLocation(skyboxProgram, "ZP");
	shaderIDs[1] = glGetUniformLocation(skyboxProgram, "ZN");
	shaderIDs[2] = glGetUniformLocation(skyboxProgram, "XP");
	shaderIDs[3] = glGetUniformLocation(skyboxProgram, "XN");
	shaderIDs[4] = glGetUniformLocation(skyboxProgram, "YP");
	shaderIDs[5] = glGetUniformLocation(skyboxProgram, "YN");
}

void SkyBox::Render(glm::mat4 VP)
{
	glCullFace(GL_FRONT);

	glUseProgram(skyboxProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texIDs[0]);
	glUniform1i(shaderIDs[0], 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texIDs[1]);
	glUniform1i(shaderIDs[1], 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texIDs[2]);
	glUniform1i(shaderIDs[2], 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texIDs[3]);
	glUniform1i(shaderIDs[3], 3);
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texIDs[4]);
	glUniform1i(shaderIDs[4], 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texIDs[5]);
	glUniform1i(shaderIDs[5], 5);

	glUniformMatrix4fv(skyboxVPMatrixID, 1, GL_FALSE, &VP[0][0]);

	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, (void*)0);
}
