#include "InputMgr.h"
#include "WindowMgr.h"

InputMgr* InputMgr::Instance;

InputMgr::InputMgr()
{
	Instance = this;
}


InputMgr::~InputMgr()
{
}

void InputMgr::UpdateInput(float deltaTime)
{
	mouseMotion = glm::vec2(0.0f);

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			quit = true;

		if (e.type == SDL_KEYDOWN)
		{
			SDL_Keycode key = e.key.keysym.sym;

			switch (key)
			{
			case SDLK_0:
				keys[KeyCode::N0] = true;
				break;

			case SDLK_1:
				keys[KeyCode::N1] = true;
				break;

			case SDLK_2:
				keys[KeyCode::N2] = true;
				break;

			case SDLK_3:
				keys[KeyCode::N3] = true;
				break;

			case SDLK_4:
				keys[KeyCode::N4] = true;
				break;

			case SDLK_5:
				keys[KeyCode::N5] = true;
				break;

			case SDLK_6:
				keys[KeyCode::N6] = true;
				break;

			case SDLK_7:
				keys[KeyCode::N7] = true;
				break;

			case SDLK_8:
				keys[KeyCode::N8] = true;
				break;

			case SDLK_9:
				keys[KeyCode::N9] = true;
				break;

			case SDLK_q:
				keys[KeyCode::Q] = true;
				break;

			case SDLK_w:
				keys[KeyCode::W] = true;
				break;

			case SDLK_a:
				keys[KeyCode::A] = true;
				break;

			case SDLK_s:
				keys[KeyCode::S] = true;
				break;

			case SDLK_d:
				keys[KeyCode::D] = true;
				break;

			case SDLK_ESCAPE:
				keys[KeyCode::ESCAPE] = true;
				break;

			case SDLK_SPACE:
				keys[KeyCode::SPACE] = true;
				break;

			default:
				break;
			}
		}

		if (e.type == SDL_KEYUP)
		{
			SDL_Keycode key = e.key.keysym.sym;

			switch (key)
			{
			case SDLK_0:
				keys[KeyCode::N0] = false;
				break;

			case SDLK_1:
				keys[KeyCode::N1] = false;
				break;

			case SDLK_2:
				keys[KeyCode::N2] = false;
				break;

			case SDLK_3:
				keys[KeyCode::N3] = false;
				break;

			case SDLK_4:
				keys[KeyCode::N4] = false;
				break;

			case SDLK_5:
				keys[KeyCode::N5] = false;
				break;

			case SDLK_6:
				keys[KeyCode::N6] = false;
				break;

			case SDLK_7:
				keys[KeyCode::N7] = false;
				break;

			case SDLK_8:
				keys[KeyCode::N8] = false;
				break;

			case SDLK_9:
				keys[KeyCode::N9] = false;
				break;

			case SDLK_q:
				keys[KeyCode::Q] = false;
				break;

			case SDLK_w:
				keys[KeyCode::W] = false;
				break;

			case SDLK_a:
				keys[KeyCode::A] = false;
				break;

			case SDLK_s:
				keys[KeyCode::S] = false;
				break;

			case SDLK_d:
				keys[KeyCode::D] = false;
				break;

			case SDLK_ESCAPE:
				keys[KeyCode::ESCAPE] = false;
				break;

			case SDLK_SPACE:
				keys[KeyCode::SPACE] = false;
				break;

			default:
				break;
			}
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			mouseMotion = glm::vec2((float)e.motion.xrel / WindowMgr::Instance->GetWidth() * 500.0f, (float)e.motion.yrel / WindowMgr::Instance->GetHeight() * 500.0f);
		}
	}

	// Smooth keys
	for (int i = 0; i < KeyCode::K_SIZE; i++)
	{
		if (keys[i] == true)
		{
			smoothKeys[i] += 3.0f * deltaTime;

			if (smoothKeys[i] > 1.0f)
				smoothKeys[i] = 1.0f;
		}

		else
		{
			smoothKeys[i] -= 3.0f * deltaTime;

			if (smoothKeys[i] < 0.0f)
				smoothKeys[i] = 0.0f;
		}
	}

	if (keys[KeyCode::ESCAPE] == true)
		quit = true;
}

float InputMgr::GetSmooth(KeyCode key)
{
	return smoothKeys[key];
}

bool InputMgr::GetKey(KeyCode key)
{
	return keys[key];
}

glm::vec2 InputMgr::GetMouseMotion()
{
	return mouseMotion;
}