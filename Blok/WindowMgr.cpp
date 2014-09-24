#include "WindowMgr.h"


WindowMgr* WindowMgr::Instance;

WindowMgr::WindowMgr()
{
	Instance = this;
}


WindowMgr::~WindowMgr()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int WindowMgr::CreateWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		sdldie("Unable to init SDL");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


	window = SDL_CreateWindow("BLOK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN// | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

	if (!window)
		sdldie("Unable to create window");

	SDL_GetWindowSize(window, &width, &height);

	checkSDLError(__LINE__);

	context = SDL_GL_CreateContext(window);
	checkSDLError(__LINE__);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	SDL_GL_SetSwapInterval(1);

	return 0;
}

void WindowMgr::SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

int WindowMgr::GetWidth()
{
	return width;
}

int WindowMgr::GetHeight()
{
	return height;
}