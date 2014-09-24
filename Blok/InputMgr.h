#pragma once

#include "SDL.h"
#include "glm\glm.hpp"

enum KeyCode
{
	Q = 0,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,
	N0,
	SPACE,
	RETURN,
	ESCAPE,
	K_SIZE,
};

class InputMgr
{
public:
	InputMgr();
	~InputMgr();

	static InputMgr* Instance;

	void UpdateInput(float deltaTime);
	float GetSmooth(KeyCode key);
	bool GetKey(KeyCode key);
	glm::vec2 GetMouseMotion();

	bool quit = false;

private:
	bool keys[KeyCode::K_SIZE];
	float smoothKeys[KeyCode::K_SIZE];
	glm::vec2 mouseMotion;
};

