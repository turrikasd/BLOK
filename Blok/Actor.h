#pragma once

#include "glm\glm.hpp"
#include <vector>

const glm::vec3	gravity = glm::vec3(0.0f, -20.0f, 0.0f);
const float jumpPower = 10.0f;

class Actor
{
public:
	Actor();
	~Actor();

	static std::vector<Actor*> actors;

	virtual void Update(float deltaTime);
	virtual void Move(float deltaTime);
	virtual void Jump();

	glm::vec3 GetPosition();
	glm::vec3 GetLook();
	glm::vec3 GetLookAt();

protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 look;

	glm::vec2 lookDeg;
};

