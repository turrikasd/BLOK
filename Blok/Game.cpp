#include "Game.h"
#include "ErrorHandler.h"
#include <iostream>

Game::Game()
{
}


Game::~Game()
{
	delete world;
	delete inputMgr;
	delete time;
}

int Game::Initialize()
{
	time = new Time();
	inputMgr = new InputMgr();
	world = new World();
	world->Initialize();

	renderer = new Renderer();
	renderer->Initialize();

	return 0;
}

int Game::Loop(WindowMgr* windowMgr)
{
	while (!inputMgr->quit)
	{
		time->UpdateTime();
		inputMgr->UpdateInput(time->deltaTime);
		world->UpdateAll(time->deltaTime);

		renderer->RenderAll(windowMgr);

		windowMgr->SwapBuffers();
		PrintGLError();
		SDL_Delay(10);
	}

	return 0;
}