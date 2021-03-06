#define _CRT_SECURE_NO_WARNINGS

#include <GL\glew.h>
#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "PerlinNoise.h"
#include <iostream>
#include <math.h>
#include <thread>

/* assimp include files. These three are usually needed. */
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "World.h"
#include "shader.hpp"
#include "TextureLoader.h"
#include "SkyBox.h"

#define PROGRAM_NAME "BLOK"

int w = 1024;
int h = 512;

bool quit = false;
Uint32 lastTime = 0;
bool run = false;

int BLOCKS0 = 0;
int BLOCKS1 = 0;
bool blockBuilderDone = true;
int blockList = -1;
GLfloat blocks0[World::MAX_BLOCKS * 4];
GLfloat blocks1[World::MAX_BLOCKS * 4];

bool buildBlocks = true;
std::thread blockBuilder;
int blockFrameCount = -1;

int CHUNK_LOAD_DIST = 32;
glm::vec3 pos = glm::vec3(World::HALF_WORLD, 257.0f, World::HALF_WORLD);
float pitch = 0.0f;
float yaw = 0.0f;

float yVel = 0.0f;

float lookX = 0.0f;
float lookZ = 0.0f;

float lookY = 0.0f;

enum KeyCode
{
	W = 0,
	S,
	D,
	A,
	Q,
	E,
	K_SIZE,
};

bool keys[KeyCode::K_SIZE];

SDL_Window* window;
World* world;
SkyBox* skyBox;

const aiScene *scene;
GLuint blokProgramID;
GLuint blockPositionBuffer;
GLuint cubeVertexBuffer;
GLuint cubeNormalBuffer;
GLuint cubeUVBuffer;

GLuint textureID;
GLuint textureUniformID;

GLuint stoneTextID;
GLuint stoneTextUniformID;

GLuint cubeVertexAttribID;
GLuint cubeUVAttribID;
GLuint cubeNormalAttribID;
GLuint cubeElementBuffer;

GLuint xyzID;
GLuint cubeVPMatrixID;

GLuint ViewMatrixID;
GLuint DepthBiasID;
GLuint ShadowMapID;


GLuint depthVertexID;
GLuint depthProgramID;
GLuint depthMatrixID;
GLuint FramebufferName = 0;
GLuint depthTexture;
GLuint depthXYZID;

GLuint cubeLightInvDirID;

int nextIndex = 0;

void PrintGLError()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s\n", errString);
		//int hold;
		//std::cin >> hold;
	}
}

/* A simple function that prints a message, the error code returned by SDL,
* and quits the application */
void sdldie(const char *msg)
{
	printf("%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}

void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}

void RenderBlocks(GLfloat* blockPositionData, GLuint vertexAttribID, GLuint xyzAttribID, int numBlocks, bool shadow)
{
	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, World::MAX_BLOCKS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, numBlocks * 4 * sizeof(GLfloat), blockPositionData);

	// Bind our texture in Texture Unit 0
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(TextureID, 0);

	//glm::mat4 Projection = glm::perspective(80.0f, (float)w / (float)h, 0.1f, 1000.0f);
	//glm::mat4 View = glm::lookAt(glm::vec3(-10, 75.0f, -10), glm::vec3(CHUNK_LOAD_DIST / 2 * World::SIDE, 50, CHUNK_LOAD_DIST / 2 * World::SIDE), glm::vec3(0, 1, 0));


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexAttribID);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glVertexAttribPointer(
		vertexAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	
	if (!shadow)
	{
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(cubeNormalAttribID);
		glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
		glVertexAttribPointer(
			cubeNormalAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);


		glEnableVertexAttribArray(cubeUVAttribID);
		glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
		glVertexAttribPointer(
			cubeUVAttribID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
	}


	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, cubeElementBuffer);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(xyzAttribID);
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionBuffer);
	glVertexAttribPointer(
		xyzAttribID,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisorARB(vertexAttribID, 0); // particles vertices : always reuse the same 4 vertices -> 0
	if (!shadow)
	{
		glVertexAttribDivisorARB(cubeNormalAttribID, 0);
		glVertexAttribDivisorARB(cubeUVAttribID, 0);
	}

	glVertexAttribDivisorARB(xyzAttribID, 1); // positions : one per quad (its center)                 -> 1                      -> 1

	// Draw the triangles !
	glDrawElementsInstancedARB(
		GL_TRIANGLES,      // mode
		scene->mMeshes[0]->mNumFaces * 3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0,           // element array buffer offset
		numBlocks
		);

	/*glDisableVertexAttribArray(vertexAttribID);
	if (!shadow)
		glDisableVertexAttribArray(cubeNormalAttribID);
	glDisableVertexAttribArray(xyzAttribID);*/
}

void AddChunkToList(int x, int z, GLfloat* blocks)
{
	//int cX = x / World::SIDE;
	//int cZ = z / World::SIDE;

	int cX = x; //x;
	int cZ = z; // z;

	bool north = false;
	bool east = false;
	bool south = false;
	bool west = false;

	char*** northBA = nullptr;
	char*** eastBA = nullptr;
	char*** southBA = nullptr;
	char*** westBA = nullptr;

	if (!world->IsGenerated(cX, cZ))
		return;

	char*** blockArray = world->chunks[cX][cZ];

	if (world->IsGenerated(cX, cZ + 1))
	{
		north = true;
		northBA = world->chunks[cX][cZ + 1];
	}

	if (world->IsGenerated(cX + 1, cZ))
	{
		east = true;
		eastBA = world->chunks[cX + 1][cZ];
	}

	if (world->IsGenerated(cX, cZ - 1))
	{
		south = true;
		southBA = world->chunks[cX][cZ - 1];
	}

	if (world->IsGenerated(cX - 1, cZ))
	{
		west = true;
		westBA = world->chunks[cX - 1][cZ];
	}

	for (int i = 0; i < World::SIDE; i++)
	{
		for (int j = 0; j < World::SIDE; j++)
		{
			for (int k = 1; k < World::HEIGHT; k++)
			{
				if (blockArray[i][j][k] != 1 && 
					(/*blockArray[i][j][k] == 5 ||
					blockArray[i][j][k] == 4 ||
					blockArray[i][j][k] == 2 ||
					blockArray[i][j][k] == 6 ||*/
					k == World::HEIGHT - 1 ||
					blockArray[i][j][k - 1] == 1 ||
					blockArray[i][j][k + 1] == 1 ||

					(i == World::SIDE - 1 && east && eastBA[0][j][k] == 1) ||
					(i == 0 && west && westBA[World::SIDE - 1][j][k] == 1) ||
					(i != World::SIDE - 1 && blockArray[i + 1][j][k] == 1) ||
					(i != 0 && blockArray[i - 1][j][k] == 1) ||

					(j == World::SIDE - 1 && north && northBA[i][0][k] == 1) ||
					(j == 0 && south && southBA[i][World::SIDE - 1][k] == 1) ||
					(j != World::SIDE - 1 && blockArray[i][j + 1][k] == 1) ||
					(j != 0 && blockArray[i][j - 1][k] == 1)))
				{
					blocks[nextIndex * 4 + 0] = cX * World::SIDE + i;
					blocks[nextIndex * 4 + 1] = k;
					blocks[nextIndex * 4 + 2] = cZ * World::SIDE + j;
					blocks[nextIndex * 4 + 3] = blockArray[i][j][k];

					nextIndex += 1;
				}
			}
		}
	}
}

void MoveVertically(float amount, Uint32 deltaTime)
{
	float newY = pos.y + amount * deltaTime;

	if (world->IsBlockAt(pos.x + 0.5f, newY, pos.z + 0.5f))
	{
		yVel = 0.0f;
		newY = pos.y;// roundf(newY);
	}

	//std::cout << newY << std::endl;
	pos.y = newY;
}

void MoveRelative(KeyCode key, Uint32 deltaTime)
{
	float MOVE = 0.01f * ((run) ? 2.0f : 1.0f);
	float movePitch = pitch;

	switch (key)
	{
	case 0:
		break;
	case 1:
		movePitch += 180;
		break;
	case 2:
		movePitch += 90;
		break;
	case 3:
		movePitch -= 90;
		break;

	default:
		break;
	}

	float pRad = movePitch * (M_PI / 180);
	float newX = pos.x + (MOVE * std::sin(pRad) * deltaTime);

	if (world->IsBlockAt(newX + 0.5f, pos.y + 1, pos.z + 0.5f) ||
		world->IsBlockAt(newX + 0.5f, pos.y + 2, pos.z + 0.5f))
	{
		newX = pos.x;
	}

	else if (world->IsBlockAt(newX + 0.5f, pos.y, pos.z + 0.5f))
	{
		pos.y += 1.05f;
	}

	pos.x = newX;

	float newZ = pos.z + (MOVE * -std::cos(pRad) * deltaTime);

	if (world->IsBlockAt(pos.x + 0.5f, pos.y + 1, newZ + 0.5f) ||
		world->IsBlockAt(pos.x + 0.5f, pos.y + 2, newZ + 0.5f))
	{
		newZ = pos.z;
	}

	else if (world->IsBlockAt(pos.x + 0.5f, pos.y, newZ + 0.5f))
	{
		pos.y += 1.05f;
	}

	pos.z = newZ;
	//pos.x += 0.5f * std::sin(pRad);
	//pos.z += 0.5f * -std::cos(pRad);
}

void BuildBlockList(GLfloat* blocks, int* blockCount)
{
	std::vector<std::thread> threads;

	if (buildBlocks)
	{
		for (int x = (pos.x - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x < (pos.x + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x++)
		{
			for (int z = (pos.z - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z < (pos.z + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z++)
			{
				if (!world->IsGenerated(x, z))
				{
					threads.push_back(std::thread(&World::GetBlocks, world, x, z));
				}
			}
		}

		for (std::vector<std::thread>::iterator t = threads.begin(); t != threads.end(); ++t)
		{
			t->join();
		}
	}

	nextIndex = 0;

	for (int x = (pos.x - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x < (pos.x + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x++)
	{
		for (int z = (pos.z - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z < (pos.z + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z++)
		{
			AddChunkToList(x, z, blocks);
		}
	}

	*blockCount = nextIndex;
	
	
	if (blockList == -1 || blockList == 1)
		blockList = 0;
	else
		blockList = 1;

	blockBuilderDone = true;
	//std::cout << "Batch of " << *blockCount << " done!" << std::endl;
}

void GenerateChunksBackground()
{
	for (;;)
	{
		if (buildBlocks)
		{
			int BG_CHUNK_LOAD_DIST = CHUNK_LOAD_DIST + 1;
			for (int x = (pos.x - (BG_CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x < (pos.x + (BG_CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x++)
			{
				if (quit || !buildBlocks)
					break;

				std::vector<std::thread> threads;

				for (int z = (pos.z - (BG_CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z < (pos.z + (BG_CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z++)
				{
					if (!world->IsGenerated(x, z))
					{
						threads.push_back(std::thread(&World::GetBlocks, world, x, z));
					}

					if (quit || !buildBlocks)
						break;
				}

				if (threads.size() == 0)
				{
					SDL_Delay(10);
					continue;
				}

				std::cout << "Background builder: building " << threads.size() << " chunks" << std::endl;
				for (std::vector<std::thread>::iterator t = threads.begin(); t != threads.end(); ++t)
				{
					t->join();
				}
				std::cout << "Background builder: Done!" << std::endl;
			}
		}

		if (quit)
			break;
	}
}

void GameLoop()
{
	world = new World();
	std::thread backgoundBuilder = std::thread(GenerateChunksBackground);

	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT){
				quit = true;
			}

			if (e.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = e.key.keysym.sym;

				switch (key)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_a:
					keys[KeyCode::A] = true;
					break;

				case SDLK_d:
					keys[KeyCode::D] = true;
					break;

				case SDLK_w:
					keys[KeyCode::W] = true;
					break;

				case SDLK_s:
					keys[KeyCode::S] = true;
					break;

				case SDLK_q:
					keys[KeyCode::Q] = true;
					break;

				case SDLK_e:
					keys[KeyCode::E] = true;
					break;

				case SDLK_SPACE:
					if (world->IsBlockAt(pos.x, pos.y - 1.0f, pos.z))
						yVel += 0.05f;
					break;

				case SDLK_f:
				{
					float pR = pitch * (M_PI / 180);
					float yR = yaw * (M_PI / 180);

					float sR = std::sin(pR);
					float cR = -std::cos(pR);

					for (int i = 1; i < 10; i++)
					{
						float lX = pos.x + i * sR;
						float lZ = pos.z + i * cR;
						float lY = pos.y + 1.8f + i * yR;

						if (world->IsBlockAt(lX + 0.5f, lY + 0.5f, lZ + 0.5f))
						{

							world->PutBlockAt(lX - sR + 0.5f, lY - yR + 0.5f, lZ - cR + 0.5f, 3);
							break;
						}
					}
					break;
				}

				case SDLK_r:
				{
					float pR = pitch * (M_PI / 180);
					float yR = yaw * (M_PI / 180);

					for (int i = 1; i < 10; i++)
					{
						float lX = pos.x + i * std::sin(pR);
						float lZ = pos.z + i * -std::cos(pR);
						float lY = pos.y + 1.8f + i * yR;

						if (world->IsBlockAt(lX, lY, lZ))
						{

							world->PutBlockAt(lX, lY, lZ, 1);
							break;
						}
					}
					break;
				}

				case SDLK_1:
					CHUNK_LOAD_DIST = 1;
					break;

				case SDLK_2:
					CHUNK_LOAD_DIST = 2;
					break;

				case SDLK_3:
					CHUNK_LOAD_DIST = 3;
					break;

				case SDLK_4:
					CHUNK_LOAD_DIST = 4;
					break;

				case SDLK_5:
					CHUNK_LOAD_DIST = 5;
					break;

				case SDLK_6:
					CHUNK_LOAD_DIST = 6;
					break;

				case SDLK_7:
					CHUNK_LOAD_DIST = 7;
					break;

				case SDLK_8:
					CHUNK_LOAD_DIST = 8;
					break;

				case SDLK_9:
					CHUNK_LOAD_DIST = 9;
					break;

				case SDLK_RETURN:
					pos = glm::vec3(World::HALF_WORLD, 257.0f, World::HALF_WORLD);
					break;

				case SDLK_x:
					glEnable(GL_BLEND);
					break;

				case SDLK_z:
					glDisable(GL_BLEND);
					break;

				case SDLK_v:
					buildBlocks = false;
					break;

				case SDLK_b:
					buildBlocks = true;
					break;

				case SDLK_n:
					SDL_SetRelativeMouseMode(SDL_TRUE);
					break;

				case SDLK_m:
					SDL_SetRelativeMouseMode(SDL_FALSE);
					break;

				case SDLK_LSHIFT:
					run = true;
					break;

				case SDLK_o:
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSize(window, &w, &h);
					break;

				case SDLK_p:
					SDL_SetWindowFullscreen(window, 0);
					SDL_GetWindowSize(window, &w, &h);
					break;
				}
			}

			if (e.type == SDL_KEYUP)
			{
				SDL_Keycode key = e.key.keysym.sym;

				switch (key)
				{
				case SDLK_a:
					keys[KeyCode::A] = false;
					break;

				case SDLK_d:
					keys[KeyCode::D] = false;
					break;

				case SDLK_w:
					keys[KeyCode::W] = false;
					break;

				case SDLK_s:
					keys[KeyCode::S] = false;
					break;

				case SDLK_q:
					keys[KeyCode::Q] = false;
					break;

				case SDLK_e:
					keys[KeyCode::E] = false;
					break;

				case SDLK_LSHIFT:
					run = false;
					break;
				}
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				pitch += e.motion.xrel * 0.3f;

				if (std::abs(yaw - e.motion.yrel * 0.6f) <= 75)
					yaw -= e.motion.yrel * 0.6f;
			}
		}

		Uint32 sdlTime = SDL_GetTicks();
		Uint32 deltaTime = sdlTime - lastTime;
		lastTime = sdlTime;
		//std::cout << deltaTime << std::endl;


		for (int i = 0; i < KeyCode::K_SIZE; i++)
		{
			if (keys[i] == true)
			{
				if (i == KeyCode::Q)
					pos.y -= 5.5f; //MoveVertically(-0.5f);
				else if (i == KeyCode::E)
					pos.y += 5.5f; //MoveVertically(0.5f);
				else
					MoveRelative((KeyCode)i, deltaTime);
			}
		}

		yVel -= 0.0002f * deltaTime;
		MoveVertically(yVel, deltaTime);

		float pitchRad = pitch * (M_PI / 180);
		float yawRad = yaw * (M_PI / 180);

		lookX = pos.x + std::sin(pitchRad);
		lookZ = pos.z + -std::cos(pitchRad);

		lookY = pos.y + yawRad;

		unsigned int wBuildStart = SDL_GetTicks();

		// Build list of blocks
		if (blockBuilderDone && (blockFrameCount == -1 || blockFrameCount >= 6))
		{
			blockBuilderDone = false;
			blockFrameCount = 0;

			if (blockList == 0)
			{
				blockBuilder = std::thread(BuildBlockList, blocks1, &BLOCKS1);
			}

			else if (blockList == -1 || 1)
			{
				blockBuilder = std::thread(BuildBlockList, blocks0, &BLOCKS0);
			}

			if (blockList != -1)
				blockBuilder.detach();
		}

		unsigned int start = SDL_GetTicks();

		// Render shadows
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1024, 1024);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(depthProgramID);

		float t = (float)SDL_GetTicks() / 10000.0;
		float x = std::sin(t);
		float z = -std::cos(t);

		glm::vec3 lightInvDir = glm::vec3(x, 0.5f, z);
		glm::vec3 lightPos = glm::vec3(pos.x + lightInvDir.x, pos.y + lightInvDir.y, pos.z + lightInvDir.z);

		// 25, 60, 110

		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-70, 70, -70, 70, -100, 200);
		glm::mat4 depthViewMatrix = glm::lookAt(lightPos, glm::vec3(pos.x, pos.y, pos.z), glm::vec3(0, 1, 0));

		//glm::mat4 depthProjectionMatrix = glm::perspective(80.0f, (float)w / (float)h, 1.0f, 10000.0f);
		//glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(pos.x, pos.y + 1.8f, pos.z), glm::vec3(lookX, lookY + 1.8f, lookZ), glm::vec3(0, 1, 0));

		glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

		if (blockList == -1)
		{
			blockBuilder.join();
		}

		if (blockList == 0)
		{
			RenderBlocks(blocks0, depthVertexID, depthXYZID, BLOCKS0, true);
		}

		else
		{
			RenderBlocks(blocks1, depthVertexID, depthXYZID, BLOCKS1, true);
		}
		
		//
		// Render image
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, w, h); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(blokProgramID);

		glm::mat4 Projection = glm::perspective(80.0f, (float)w / (float)h, 0.1f, 10000.0f);
		glm::mat4 View = glm::lookAt(glm::vec3(pos.x, pos.y + 1.8f, pos.z), glm::vec3(lookX, lookY + 1.8f, lookZ), glm::vec3(0, 1, 0));

		glm::mat4 VP = Projection * View;
		glUniformMatrix4fv(cubeVPMatrixID, 1, GL_FALSE, &VP[0][0]);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

		//glUniform3f(cubeLightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);
		glUniform3f(cubeLightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(textureUniformID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, stoneTextID);
		glUniform1i(stoneTextUniformID, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(ShadowMapID, 2);


		if (blockList == 0)
		{
			RenderBlocks(blocks0, cubeVertexAttribID, xyzID, BLOCKS0, false);
		}
		
		else
		{
			RenderBlocks(blocks1, cubeVertexAttribID, xyzID, BLOCKS1, false);
		}


		skyBox->Render(scene, VP);
		
		SDL_GL_SwapWindow(window);
		blockFrameCount++;

		//std::cout << (SDL_GetTicks() - wBuildStart) << std::endl;

		SDL_Delay(10);
		checkSDLError(__LINE__);
		PrintGLError();
	}	// END LOOP

	backgoundBuilder.join();
}

bool InitResources()
{
	glClearColor(0.74f, 0.79f, 1.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and compile our GLSL program from the shaders
	blokProgramID = LoadShaders("Blok.vert", "Blok.frag");
	//blokProgramID = LoadShadersGS("Blok.vert", "Blok.frag", "Blok.gs");

	scene = aiImportFile("cube.obj", aiProcessPreset_TargetRealtime_MaxQuality);
	unsigned int *faceArray;
	faceArray = (unsigned int*)malloc(sizeof(unsigned int) * scene->mMeshes[0]->mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < scene->mMeshes[0]->mNumFaces; ++t)
	{
		const aiFace* face = &scene->mMeshes[0]->mFaces[t];

		memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}

	cubeVertexAttribID = glGetAttribLocation(blokProgramID, "in_Position");
	cubeUVAttribID = glGetAttribLocation(blokProgramID, "in_UV");
	cubeNormalAttribID = glGetAttribLocation(blokProgramID, "in_Normal");
	xyzID = glGetAttribLocation(blokProgramID, "xyzt");

	cubeVPMatrixID = glGetUniformLocation(blokProgramID, "VP");

	textureID = LoadTexture("grass.png");
	stoneTextID = LoadTexture("stone.png");
	textureUniformID = glGetUniformLocation(blokProgramID, "mainText");
	stoneTextUniformID = glGetUniformLocation(blokProgramID, "stoneText");

	// Get a handle for our "MVP" uniform
	ViewMatrixID = glGetUniformLocation(blokProgramID, "V");
	DepthBiasID = glGetUniformLocation(blokProgramID, "DepthBiasMVP");
	ShadowMapID = glGetUniformLocation(blokProgramID, "shadowMap");
	cubeLightInvDirID = glGetUniformLocation(blokProgramID, "LightInvDirection_worldspace");

	glGenBuffers(1, &cubeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeUVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mTextureCoords[0], GL_STATIC_DRAW);

	glGenBuffers(1, &cubeNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mNormals, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->mMeshes[0]->mNumFaces * 3 * sizeof(GLfloat), faceArray, GL_STATIC_DRAW);


	// The VBO containing the positions and sizes of the blocks
	glGenBuffers(1, &blockPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, blockPositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, World::MAX_BLOCKS * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	
	//
	// Stuff for shadows
	depthProgramID = LoadShaders("Depth.vert", "Depth.frag");

	// Get a handle for our "MVP" uniform
	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");
	depthVertexID = glGetAttribLocation(depthProgramID, "in_Position");
	depthXYZID = glGetAttribLocation(depthProgramID, "xyzt");


	// ---------------------------------------------
	// Render to Texture - specific code begins here
	// ---------------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;


	skyBox = new SkyBox();
	skyBox->Load();

	return true;
}

void ClearResources()
{

}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		sdldie("Unable to init SDL");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	

	window = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN// | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

	if (!window)
		sdldie("Unable to create window");

	SDL_GetWindowSize(window, &w, &h);

	//SDL_SetRelativeMouseMode(SDL_TRUE);

	checkSDLError(__LINE__);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	checkSDLError(__LINE__);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	SDL_GL_SetSwapInterval(1);

	if (!InitResources())
	{
		int hold;
		std::cout << "Major failure at Init" << std::endl;
		std::cin >> hold;
	}

	//std::cin >> CHUNK_LOAD_DIST;
	CHUNK_LOAD_DIST = 1;// 56;

	GameLoop();
	ClearResources();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}