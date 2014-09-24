#include <iostream>

#include "WindowMgr.h"
#include "Game.h"

int main(int argc, char *argv[])
{
	int res;

	WindowMgr *windowMgr = new WindowMgr();
	res = windowMgr->CreateWindow();

	Game *game = new Game();
	res = game->Initialize();
	res = game->Loop(windowMgr);

	delete game;
	delete windowMgr;

	return 0;
}