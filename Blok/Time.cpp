#include "Time.h"


Time::Time()
{
	deltaTime = 0.016f;
	fixedTime = 0.0f;

	lastTime = 0;
}


Time::~Time()
{
}

void Time::UpdateTime()
{
	Uint32 current = SDL_GetTicks();
	Uint32 difference = current - lastTime;

	deltaTime = difference / 1000.0f;
	fixedTime = current / 1000.0f;

	lastTime = current;
}
