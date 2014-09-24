#pragma once

#include "GL\glew.h"
#include "TextureLoader.h"
#include "shader.hpp"

#include "WorldRenderer.h"
#include "WindowMgr.h"

#include "SkyBox.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Initialize();
	void RenderAll(WindowMgr* windowMgr);

private:
	WorldRenderer* worldRenderer;
};

