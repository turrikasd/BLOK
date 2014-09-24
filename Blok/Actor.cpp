#include "Actor.h"
#include "World.h"

std::vector<Actor*> Actor::actors;

Actor::Actor()
{
	position = glm::vec3(World::HALF_WORLD, World::HEIGHT, World::HALF_WORLD);
	actors.push_back(this);
}


Actor::~Actor()
{
}

void Actor::Update(float deltaTime)
{
	//velocity *= 0.9f;
	velocity += gravity * deltaTime;
	Move(deltaTime);
}

void Actor::Move(float deltaTime)
{
	World* w = World::Instance;

	glm::vec3 newPos = position + velocity * deltaTime;

	if (w->IsBlockAt(newPos.x, position.y - 1.0f, position.z))
	{
		velocity.x = 0.0f;
	}

	if (w->IsBlockAt(position.x, newPos.y - 1.0f, position.z))
	{
		velocity.y = 0.0f;
	}

	if (w->IsBlockAt(position.x, position.y - 1.0f, newPos.z))
	{
		velocity.z = 0.0f;
	}

	position += velocity * deltaTime;
}

glm::vec3 Actor::GetPosition()
{
	return position;
}

glm::vec3 Actor::GetLook()
{
	return look;
}

glm::vec3 Actor::GetLookAt()
{
	return position + look;
}

void Actor::Jump()
{
	if (World::Instance->IsBlockAt(position.x, position.y - 1.5f, position.z))
		velocity.y = jumpPower;
}