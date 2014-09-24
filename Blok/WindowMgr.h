#pragma once

#include "GL\glew.h"
#include "SDL.h"
#include "ErrorHandler.h"

class WindowMgr
{
public:
	WindowMgr();
	~WindowMgr();

	static WindowMgr* Instance;

	int CreateWindow();
	void SwapBuffers();
	int GetWidth();
	int GetHeight();

private:
	SDL_Window* window;
	SDL_GLContext context;
	int width = 1024;
	int height = 512;
};

