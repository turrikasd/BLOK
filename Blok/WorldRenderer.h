#pragma once

#include "GL\glew.h"
#include "TextureLoader.h"
#include "shader.hpp"
#include "ModelLoader.h"
#include "World.h"
#include "glm\gtx\transform.hpp"
#include "WindowMgr.h"
#include "SkyBox.h"

class WorldRenderer
{
public:
	WorldRenderer();
	~WorldRenderer();

	void Initialize();
	void Render(WindowMgr* windowMgr);

private:
	SkyBox* skybox;

	GLuint blockProgramID;

	GLuint blockVertexBuffer;
	GLuint blockUVBuffer;
	GLuint blockNormalBuffer;
	GLuint blockElementBuffer;
	GLuint blockNumTriangles;
	GLuint blockPositionTypeBuffer;

	GLuint blockVertexAttribID;
	GLuint blockUVAttribID;
	GLuint blockNormalAttribID;
	GLuint xyztID;

	GLuint blockVPMatrixID;
	GLuint blockViewMatrixID;
	GLuint blockDepthBiasID;
	GLuint blockShadowMapID;
	GLuint blockLightInvDirID;

	GLuint stoneTexture;
	GLuint blockStoneTextID;

	GLuint depthProgramID;
	GLuint depthMatrixID;
	GLuint depthVertexID;
	GLuint depthXYZTID;
	
	GLuint FramebufferName = 0;
	GLuint depthTexture;

	GLfloat* blocks;
	GLuint numBlocks;
};

