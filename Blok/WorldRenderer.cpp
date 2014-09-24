#include "WorldRenderer.h"


WorldRenderer::WorldRenderer()
{
}


WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::Initialize()
{
	blockProgramID = LoadShaders("Blok.vert", "Blok.frag");
	LoadModelElem("cube.obj", &blockVertexBuffer, &blockUVBuffer, &blockNormalBuffer, &blockElementBuffer, &blockNumTriangles);

	glGenBuffers(1, &blockPositionTypeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionTypeBuffer);
	glBufferData(GL_ARRAY_BUFFER, World::MAX_BLOCKS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	blockVertexAttribID = glGetAttribLocation(blockProgramID, "in_Position");
	blockUVAttribID = glGetAttribLocation(blockProgramID, "in_UV");
	blockNormalAttribID = glGetAttribLocation(blockProgramID, "in_Normal");
	xyztID = glGetAttribLocation(blockProgramID, "xyzt");

	stoneTexture = LoadTexture("stone.png");
	blockStoneTextID = glGetUniformLocation(blockProgramID, "stoneText");

	blockVPMatrixID = glGetUniformLocation(blockProgramID, "VP");
	blockViewMatrixID = glGetUniformLocation(blockProgramID, "V");
	blockDepthBiasID = glGetUniformLocation(blockProgramID, "DepthBiasMVP");
	blockShadowMapID = glGetUniformLocation(blockProgramID, "shadowMap");
	blockLightInvDirID = glGetUniformLocation(blockProgramID, "LightInvDirection_worldspace");

	// ----
	depthProgramID = LoadShaders("Depth.vert", "Depth.frag");
	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
	depthVertexID = glGetAttribLocation(depthProgramID, "in_Position");
	depthXYZTID = glGetAttribLocation(depthProgramID, "xyzt");

	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// ERRRRR
	}

	skybox = new SkyBox();
	skybox->Load();
}

#include <iostream>
void WorldRenderer::Render(WindowMgr* windowMgr)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1024, 1024);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(depthProgramID);

	glm::vec3 lightInvDir = glm::vec3(2.0f, 0.5f, 2.0f);
	glm::vec3 lightPos = Player::instance->GetPosition() + lightInvDir;

	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-70, 70, -70, 70, -100, 200);
	glm::mat4 depthViewMatrix = glm::lookAt(lightPos, Player::instance->GetPosition(), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

	WorldBuilder::instance->GetBatch(&blocks, &numBlocks);


	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionTypeBuffer);
	glBufferData(GL_ARRAY_BUFFER, World::MAX_BLOCKS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, numBlocks * 4 * sizeof(GLfloat), blocks);

	glEnableVertexAttribArray(depthVertexID);
	glBindBuffer(GL_ARRAY_BUFFER, blockVertexBuffer);
	glVertexAttribPointer(
		depthVertexID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glBindBuffer(GL_ARRAY_BUFFER, blockElementBuffer);

	glEnableVertexAttribArray(depthXYZTID);
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionTypeBuffer);
	glVertexAttribPointer(
		depthXYZTID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glVertexAttribDivisorARB(depthVertexID, 0);
	glVertexAttribDivisorARB(depthXYZTID, 1);


	glDrawElementsInstancedARB(
		GL_TRIANGLES,      // mode
		blockNumTriangles * 3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0,           // element array buffer offset
		numBlocks
		);



	// --- Render Image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowMgr->GetWidth(), windowMgr->GetHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(blockProgramID);

	glm::mat4 Projection = glm::perspective(80.0f, (float)windowMgr->GetWidth() / (float)windowMgr->GetHeight(), 0.1f, 10000.0f);
	glm::mat4 View = glm::lookAt(Player::instance->GetPosition(), Player::instance->GetLookAt(), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 View = glm::lookAt(glm::vec3(4000, 300, 4000), glm::vec3(4100, 64, 4100), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 VP = Projection * View;


	glUniformMatrix4fv(blockVPMatrixID, 1, GL_FALSE, &VP[0][0]);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
	glUniformMatrix4fv(blockDepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
	glUniformMatrix4fv(blockViewMatrixID, 1, GL_FALSE, &View[0][0]);

	glUniform3f(blockLightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

	// Textures

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, stoneTexture);
	glUniform1i(blockStoneTextID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(blockShadowMapID, 1);

	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionTypeBuffer);
	glBufferData(GL_ARRAY_BUFFER, World::MAX_BLOCKS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, numBlocks * 4 * sizeof(GLfloat), blocks);

	glEnableVertexAttribArray(blockVertexAttribID);
	glBindBuffer(GL_ARRAY_BUFFER, blockVertexBuffer);
	glVertexAttribPointer(
		blockVertexAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(blockNormalAttribID);
	glBindBuffer(GL_ARRAY_BUFFER, blockNormalBuffer);
	glVertexAttribPointer(
		blockNormalAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glEnableVertexAttribArray(blockUVAttribID);
	glBindBuffer(GL_ARRAY_BUFFER, blockUVBuffer);
	glVertexAttribPointer(
		blockUVAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glBindBuffer(GL_ARRAY_BUFFER, blockElementBuffer);

	glEnableVertexAttribArray(xyztID);
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionTypeBuffer);
	glVertexAttribPointer(
		xyztID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glVertexAttribDivisorARB(blockVertexAttribID, 0);
	glVertexAttribDivisorARB(blockNormalAttribID, 0);
	glVertexAttribDivisorARB(blockUVAttribID, 0);
	glVertexAttribDivisorARB(xyztID, 1);

	glDrawElementsInstancedARB(
		GL_TRIANGLES,      // mode
		blockNumTriangles * 3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0,           // element array buffer offset
		numBlocks
		);

	skybox->Render(VP);
}