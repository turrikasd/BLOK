#pragma once

#include "SDL.h"

class Time
{
public:
	Time();
	~Time();

	void UpdateTime();

	float deltaTime;
	float fixedTime;

private:
	Uint32 lastTime;
};

