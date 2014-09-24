#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
	delete worldRenderer;
}

void Renderer::Initialize()
{
	worldRenderer = new WorldRenderer();
	worldRenderer->Initialize();

	glClearColor(0.74f, 0.79f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::RenderAll(WindowMgr* windowMgr)
{
	worldRenderer->Render(windowMgr);
}