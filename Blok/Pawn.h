#pragma once
#include "Actor.h"
class Pawn :
	public Actor
{
public:
	Pawn();
	~Pawn();

	void Update(float deltaTime);
};

