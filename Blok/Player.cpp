#include "Player.h"
#include <math.h>

Player* Player::instance;

Player::Player()
{
	instance = this;
}


Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	HandleInput(deltaTime);
	Pawn::Update(deltaTime);
}

void Player::HandleInput(float deltaTime)
{
	glm::vec2 mouseMotion = InputMgr::Instance->GetMouseMotion();
	
	lookDeg.x += mouseMotion.x;
	lookDeg.y += -mouseMotion.y;

	if (lookDeg.y > 75.0f)
		lookDeg.y = 75.0f;
	else if (lookDeg.y < -75.0f)
		lookDeg.y = -75.0f;

	float pitchRad = lookDeg.x * (M_PI / 180);
	float yawRad = lookDeg.y * (M_PI / 180);

	look.x += std::sin(pitchRad);
	look.z += -std::cos(pitchRad);

	look.y += yawRad;

	look = glm::normalize(look);

	glm::vec2 forward = glm::vec2(look.x, look.z) * InputMgr::Instance->GetSmooth(KeyCode::W);
	glm::vec2 backward = glm::vec2(-look.x, -look.z) * InputMgr::Instance->GetSmooth(KeyCode::S);

	glm::vec2 right = glm::vec2(-look.z, look.x) * InputMgr::Instance->GetSmooth(KeyCode::D);
	glm::vec2 left = glm::vec2(look.z, -look.x) * InputMgr::Instance->GetSmooth(KeyCode::A);

	velocity.x = (forward.x + backward.x + right.x + left.x) * 10.0f;
	velocity.z = (forward.y + backward.y + right.y + left.y) * 10.0f;


	if (InputMgr::Instance->GetKey(KeyCode::SPACE))
	{
		Jump();
	}
}