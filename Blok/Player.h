#pragma once
#include "Pawn.h"
#include "InputMgr.h"

class Player :
	public Pawn
{
public:
	Player();
	~Player();

	static Player* instance;
	void Update(float deltaTime);
	void HandleInput(float deltaTime);
};

