#pragma once

#include "World.h"
#include "InputMgr.h"
#include "Time.h"
#include "Renderer.h"
#include "WindowMgr.h"

class Game
{
public:
	Game();
	~Game();

	int Initialize(); 
	int Loop(WindowMgr* windowMgr);

private:
	Time* time;
	InputMgr* inputMgr;
	World* world;

	Renderer* renderer;
};

