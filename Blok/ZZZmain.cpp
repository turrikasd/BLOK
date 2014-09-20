#define _CRT_SECURE_NO_WARNINGS

#include <GL\glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "PerlinNoise.h"
#include <iostream>
#include <math.h>

/* assimp include files. These three are usually needed. */
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Chunk.h"
#include "shader.hpp"

#define PROGRAM_NAME "BLOK"

int w = 1024;
int h = 512;

Chunk* chunk;

SDL_Window *mainwindow; /* Our window handle */
SDL_GLContext maincontext; /* Our opengl context handle */

int i; /* Simple iterator */
GLuint vao, vbo[3]; /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */
int IsCompiled_VS, IsCompiled_FS;
int IsLinked;
int maxLength;
char *vertexInfoLog;
char *fragmentInfoLog;
char *shaderProgramInfoLog;

GLuint MatrixID;
GLuint MainTextID;
GLuint TextID;
GLuint PosID;
GLuint LightID;
// Generate a buffer for the indices
GLuint elementbuffer;
GLuint depthTexture;
GLuint FramebufferName;
GLuint ShadowPosID;

GLuint depthProgramID;
GLuint depthMatrixID;
GLuint ShadowMapID;

GLuint DepthBiasID;

glm::mat4 depthMVP;

/* the global Assimp scene object */
const struct aiScene* scene = NULL;

int chunkLoadDist = 128;


#define VERT 0
#define UV 1
#define NORM 2

const GLuint cubeVertCount = 8 * 3 * 3;
const GLfloat Cube[8 * 3][3] = {
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,

	0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, -0.5,

	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,

	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,

	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, -0.5,

	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5,

	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	-0.5, -0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5
};

const GLuint cubeIndiCount = 4 * 3 * 9;
const GLuint CubeIndices[4 * 3 * 9] = {
	3, 1, 1, 4, 2, 1, 2, 3, 1,
	7, 4, 2, 8, 5, 2, 6, 6, 2,
	12, 7, 3, 11, 8, 3, 9, 9, 3,

	15, 10, 4, 16, 11, 4, 14, 12, 4,
	20, 13, 5, 19, 14, 5, 18, 15, 5,
	21, 16, 6, 22, 17, 6, 23, 18, 6,

	1, 19, 1, 3, 1, 1, 2, 3, 1,
	5, 20, 2, 7, 4, 2, 6, 6, 2,
	10, 21, 3, 12, 7, 3, 9, 9, 3,

	13, 22, 4, 15, 10, 4, 14, 12, 4,
	17, 23, 5, 20, 13, 5, 18, 15, 5,
	24, 24, 6, 21, 16, 6, 23, 18, 6
};

const GLuint cubeNormCount = 6 * 4 * 3;
const GLfloat CubeNormals[6 * 4][3] = {
	0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
	0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
	1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
	0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0
};

// Element Indicies for the Cube
/*const GLuint CubeIndices[2 * 3 * 6] = {
		0, 1, 2, 2, 3, 0,
		3, 2, 6, 6, 7, 3,
		7, 6, 5, 5, 4, 7,

		4, 0, 3, 3, 7, 4,
		0, 1, 5, 5, 4, 0,
		1, 5, 6, 6, 2, 1
};

const GLfloat Cube[2 * 4][3] = {
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,

	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5 };
*/

const GLuint cubeUVCount = 2 * 4 * 2;
static const GLfloat CubeUV[2 * 4][2] = {
	0.000000f, 1.000000f,
	1.000000f, 0.000000f,
	0.000000f, 0.000000f,
	1.000000f, 1.000000f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};

/* These pointers will receive the contents of our shader source code files */
GLchar *vertexsource, *fragmentsource;

/* These are handles used to reference the shaders */
GLuint vertexshader, fragmentshader;

/* This is a handle to the shader program */
GLuint shaderprogram;


class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

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

Vector3 pos(Chunk::WORLD_SIZE / 2, 65.0f, Chunk::WORLD_SIZE / 2);
float pitch = 0;
float yaw = 0;

float yVel = 0.0f;

bool keys[KeyCode::K_SIZE];

void PrintGLError()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s\n", errString);
	}
}

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
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

void DrawCube(int x, int y, int z, bool shadow)
{
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Model[0][0]);
	GLfloat bPos[3] = { x, y, z };

	if (shadow)
		glUniform3fv(ShadowPosID, 1, bPos);
	else
		glUniform3fv(PosID, 1, bPos);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		scene->mMeshes[0]->mNumFaces * 3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
		);

	//glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
}

void RenderChunk(int x, int z, bool shadow)
{
	//int cX = x / Chunk::SIDE;
	//int cZ = z / Chunk::SIDE;

	int cX = x; //x;
	int cZ = z; // z;

	int cBlockIndex = chunk->GetBlocks(cX, cZ);

	for (int i = 0; i < Chunk::SIDE; i++)
	{
		for (int j = 0; j < Chunk::SIDE; j++)
		{
			//int y = chunk->blocks[cBlockIndex + Chunk::SIDE * i + j];
			//int ind = Chunk::PosToIndex(cX * Chunk::SIDE + i, cZ * Chunk::SIDE + j);
			//int y = chunk->blocks[ind];

			int y = chunk->blocks[cX][cZ][i * Chunk::SIDE + j];

			//std::cout << y << std::endl;

			int max = y - 1;
			y -= 0;

			while (y > max)
			{
				//int * dx = new int;
				//int * dz = new int;

				//chunk->IndexToPos(ind, dx, dz);
				//DrawCube(*dx, y, *dz, Projection, View);

				DrawCube(cX * Chunk::SIDE + i, y, cZ * Chunk::SIDE + j, shadow);
				y--;
			}
		}
	}
}

void RenderShadows(glm::mat4 proj, glm::mat4 view, float lookX, float lookY, float lookZ)
{
	PrintGLError();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glViewport(0, 0, 512, 512);

	// Backfaces??

	glClearColor(0.749f, 0.749f, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(depthProgramID);

	PrintGLError();

	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	// Compute the MVP matrix from the light's point of view
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	//glm::mat4 depthProjectionMatrix = glm::perspective(80.0f, (float)w / (float)h, 1.0f, 10000.0f);
	//glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	float pitchRad = pitch * (M_PI / 180);
	float yawRad = yaw * (M_PI / 180);

	float lX = Chunk::WORLD_SIZE / 2;// std::sin(pitchRad) * 30;
	float lZ = Chunk::WORLD_SIZE / 2;// -std::cos(pitchRad) * 30;

	//glm::mat4 View = glm::lookAt(glm::vec3(pos.x, pos.y + 5.0f, pos.z), glm::vec3(lookX, lookY + 5.0f, lookZ), glm::vec3(0, 1, 0));
	glm::mat4 View = glm::lookAt(glm::vec3(lX, 65.0f, lZ), glm::vec3(pos.x, pos.y + 0.0f, pos.z), glm::vec3(0, 1, 0));

	depthMVP = proj * View;

	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	
	glm::vec3 pLook = glm::vec3(lookX - pos.x, lookY - pos.y, lookZ - pos.z);
	pLook = glm::normalize(pLook);

	for (int x = (pos.x - (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; x < (pos.x + (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; x++)
	{
		for (int z = (pos.z - (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; z < (pos.z + (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; z++)
		{
			RenderChunk(x, z, true);
		}
	}
}

void RenderCubes(int x, int z)
{
	int sPos = glGetAttribLocation(shaderprogram, "bPos");
	int pos1 = sPos + 0;
	glEnableVertexAttribArray(pos1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glVertexAttribPointer(pos1, 3, GL_INT, GL_FALSE, 0, 0);
	glVertexAttribIPointer(pos1, 3, GL_INT, GL_FALSE, 0);

	glVertexAttribDivisor(pos1, 1);

	/*GLfloat bs[6] = { 0.0, 0.0, 0.0,
		1.0, 0.0, 0.0 };*/

	unsigned int NumInstances = Chunk::SIDE * Chunk::SIDE;
	chunk->GetBlocks(x, z);

	GLint blockPositions[Chunk::SIDE][Chunk::SIDE][3];
	for (int i = 0; i < Chunk::SIDE; i++)
	{
		for (int j = 0; j < Chunk::SIDE; j++)
		{
			blockPositions[i][j][0] = i;
			blockPositions[i][j][1] = chunk->blocks[x][z][i * Chunk::SIDE + j];
			blockPositions[i][j][2] = j;

			//std::cout << i << " : " << chunk->blocks[x][z][i * Chunk::SIDE + j] << " : " << j << std::endl;
		}
	}

	//std::cin >> NumInstances;

	glBufferData(GL_ARRAY_BUFFER, NumInstances * 3 * sizeof(GLint), blockPositions, GL_DYNAMIC_DRAW);

	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * NumInstances, bs, GL_DYNAMIC_DRAW);

	//glBindVertexArray(m_VAO);

	glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
		2 * 3 * 6,
		GL_UNSIGNED_INT,
		(void*)0 ,          // element array buffer offset
		NumInstances,
		0);

	// Make sure the VAO is not changed from the outside
	//glBindVertexArray(0);
}

void MoveVertically(float amount)
{
	float newY = pos.y + amount;

	if (chunk->IsBlockAt(pos.x + 0.5f, newY, pos.z + 0.5f))
	{
		yVel = 0.0f;
		newY = pos.y;// roundf(newY);
	}

	pos.y = newY;
}

void MoveRelative(KeyCode key)
{
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
	float newX = pos.x + 0.5f * std::sin(pRad);

	if (chunk->IsBlockAt(newX + 0.5f, pos.y, pos.z + 0.5f))
	{
		//newX = pos.x;// roundf(newX + 0.5f);
		pos.y += 1.05f;
	}

	pos.x = newX;

	float newZ = pos.z + 0.5f * -std::cos(pRad);

	if (chunk->IsBlockAt(pos.x + 0.5f, pos.y, newZ + 0.5f))
	{
		//newZ = pos.z;// roundf(newZ + 0.5f);
		pos.y += 1.05f;
	}

	pos.z = newZ;
	//pos.x += 0.5f * std::sin(pRad);
	//pos.z += 0.5f * -std::cos(pRad);
}

void GameLoop()
{
	//SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	bool quit = false;

	chunk = new Chunk();

	GLfloat Light[3] = {
		0.65, 0.82, 0.5f
	};

	glUniform3fv(LightID, 1, Light);

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
					if (yVel == 0.0f)
						yVel += 1.0f;
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
				}
			}

			if (e.type == SDL_MOUSEMOTION)
			{
				pitch += e.motion.xrel * 0.3f;

				if (std::abs(yaw - e.motion.yrel * 0.6f) <= 75)
					yaw -= e.motion.yrel * 0.6f;
			}
		}

		for (int i = 0; i < KeyCode::K_SIZE; i++)
		{
			if (keys[i] == true)
			{
				if (i == KeyCode::Q)
					MoveVertically(-0.5f);
				else if (i == KeyCode::E)
					pos.y += 0.5f; //MoveVertically(0.5f);
				else
					MoveRelative((KeyCode)i);
			}
		}

		yVel -= 0.15f;
		MoveVertically(yVel);

		float pitchRad = pitch * (M_PI / 180);
		float yawRad = yaw * (M_PI / 180);

		float lookX = pos.x + std::sin(pitchRad);
		float lookZ = pos.z + -std::cos(pitchRad);

		float lookY = pos.y + yawRad;

		glm::mat4 Projection = glm::perspective(80.0f, (float)w / (float)h, 1.0f, 10000.0f);
		//glm::mat4 View = glm::lookAt(glm::vec3(pos.x, pos.y + 5.0f, pos.z), glm::vec3(lookX, lookY + 5.0f, lookZ), glm::vec3(0, 1, 0));
		glm::mat4 View = glm::lookAt(glm::vec3(pos.x, pos.y + 5.0f, pos.z), glm::vec3(lookX, lookY + 5.0f, lookZ), glm::vec3(0, 1, 0));
		//glm::mat4 View = glm::lookAt(camPos, camLook, glm::vec3(0, 1, 0));
		//glm::mat4 View = glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		//DEBUG DEBUG
		//RenderShadows(Projection, View, lookX, lookY, lookZ);
		//SDL_GL_SwapWindow(mainwindow);
		//SDL_Delay(10);
		//continue;

		glm::mat4 MVP = Projection * View;


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, w, h); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Culling ??

		// Use our shader
		glUseProgram(shaderprogram);

		glClearColor(0.749f, 0.749f, 1.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

		// Send our transformation to the currently bound shader, 
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(ShadowMapID, 1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		unsigned int s = SDL_GetTicks();

		glm::vec3 pLook = glm::vec3(lookX - pos.x, lookY - pos.y, lookZ - pos.z);
		pLook = glm::normalize(pLook);

		for (int x = (pos.x - (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; x < (pos.x + (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; x++)
		{
			for (int z = (pos.z - (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; z < (pos.z + (chunkLoadDist * Chunk::SIDE)) / Chunk::SIDE; z++)
			{
				if ((int)(pos.x / Chunk::SIDE) != x || (int)(pos.z / Chunk::SIDE) != z)
				{
					glm::vec3 cLook = glm::vec3(pos.x - x * Chunk::SIDE, 0, pos.z - z * Chunk::SIDE);
					cLook = glm::normalize(cLook);
					float dot = glm::dot(pLook, cLook);

					if (dot > -0.1)
					{
						continue;
					}
				}

				RenderChunk(x, z, false);
			}
		}

		DrawCube(Chunk::WORLD_SIZE / 2, 65, Chunk::WORLD_SIZE / 2, false);

		//std::cout << SDL_GetTicks() - s << std::endl;

		//RenderCubes(0, 0);
		//DrawCube(0, 0, 0, Projection, View);

		SDL_GL_SwapWindow(mainwindow);
		SDL_Delay(10);
	}
}

GLuint loadBMP(const char * imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Image could not be opened\n"); return 0;
	}

	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return textureID;
}

void LoadArrays()
{
	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays(1, &vao);

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray(vao);

	/* Allocate and assign two Vertex Buffer Objects to our handle */
	glGenBuffers(3, vbo);

	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	/* Copy the vertex data from diamond to our buffer */
	/* 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values */
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mVertices, GL_STATIC_DRAW);
	/* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);

	/* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	/* Copy the color data from colors to our buffer */
	/* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mTextureCoords[0], GL_STATIC_DRAW);
	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[0]->mNumVertices * 3 * sizeof(GLfloat), scene->mMeshes[0]->mNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// BLOCK POSITION BUFFERS
	/*glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);*/

	/* Read our shaders into the appropriate buffers */
	vertexsource = filetobuf("block.vert");
	fragmentsource = filetobuf("block.frag");

	/* Create an empty vertex shader handle */
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	/* Send the vertex shader source code to GL */
	/* Note that the source code is NULL character terminated. */
	/* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	/* Compile the vertex shader */
	glCompileShader(vertexshader);

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == FALSE)
	{
		glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		vertexInfoLog = (char *)malloc(maxLength);

		std::vector<char> VertexShaderErrorMessage(maxLength);
		glGetShaderInfoLog(vertexshader, maxLength, &maxLength, &VertexShaderErrorMessage[0]);

		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

		free(vertexInfoLog);
		return;
	}

	/* Create an empty fragment shader handle */
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	/* Send the fragment shader source code to GL */
	/* Note that the source code is NULL character terminated. */
	/* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	/* Compile the fragment shader */
	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == FALSE)
	{
		glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		fragmentInfoLog = (char *)malloc(maxLength);

		std::vector<char> FragmentShaderErrorMessage(maxLength);
		glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

		free(fragmentInfoLog);
		return;
	}

	/* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
	/* We must link them together to make a GL shader program */
	/* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
	/* Assign our program handle a "name" */
	shaderprogram = glCreateProgram();

	/* Attach our shaders to our program */
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);

	/* Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color */
	/* Attribute locations must be setup before calling glLinkProgram. */
	glBindAttribLocation(shaderprogram, 0, "in_Position");
	glBindAttribLocation(shaderprogram, 1, "vertexUV");
	glBindAttribLocation(shaderprogram, 2, "in_Normal");

	/* Link our program */
	/* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
	/* The binary code is uploaded to the GPU, if there is no error. */
	glLinkProgram(shaderprogram);

	/* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
	/* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
	/* too many texel fetch instructions or too many interpolators or dynamic loops. */

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == FALSE)
	{
		/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
		glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		shaderProgramInfoLog = (char *)malloc(maxLength);

		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		free(shaderProgramInfoLog);
		return;
	}

	/* Load the shader into the rendering pipeline */
	glUseProgram(shaderprogram);

	MatrixID = glGetUniformLocation(shaderprogram, "MVP");
	DepthBiasID = glGetUniformLocation(shaderprogram, "DepthBiasMVP");
	PosID = glGetUniformLocation(shaderprogram, "BlockPos");
	MainTextID = glGetUniformLocation(shaderprogram, "mainText");
	LightID = glGetUniformLocation(shaderprogram, "light");

	TextID = loadBMP("grass.bmp");

	unsigned int *faceArray;
	faceArray = (unsigned int*)malloc(sizeof(unsigned int) * scene->mMeshes[0]->mNumFaces * 3);
	unsigned int faceIndex = 0;

	for (unsigned int t = 0; t < scene->mMeshes[0]->mNumFaces; ++t)
	{
		const aiFace* face = &scene->mMeshes[0]->mFaces[t];

		memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	}

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->mMeshes[0]->mNumFaces * sizeof(unsigned int) * 3, faceArray, GL_STATIC_DRAW);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	depthProgramID = LoadShaders("Depth.vert", "Depth.frag");

	depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	PrintGLError();
	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.*/
	glReadBuffer(GL_NONE);

	PrintGLError();

	// Always check that our framebuffer is ok
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		std::cout << "Endifned" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "Incomplete attachment" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "Incomplete draw buffer" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "Incomplete read buffer" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cout << "Incomplete multisample" << std::endl;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		std::cout << "Incomplete layer targets" << std::endl;
		break;

	default:
		std::cout << "Default" << std::endl;
		break;
	}

	PrintGLError();

	
	ShadowPosID = glGetUniformLocation(depthProgramID, "BlockPos");

	ShadowMapID = glGetUniformLocation(shaderprogram, "shadowMap");
}

void OpenGLCleanup()
{
	/* Cleanup all the things we bound and allocated */
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDetachShader(shaderprogram, vertexshader);
	glDetachShader(shaderprogram, fragmentshader);
	glDeleteProgram(shaderprogram);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
	free(vertexsource);
	free(fragmentsource);
}

/* Our program's entry point */
int ZZZmain(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
		sdldie("Unable to initialize SDL"); /* Or die on error */

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	/*
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	*/

	/* Create our window centered at 512x512 resolution */
	mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN //| SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	if (!mainwindow) /* Die if creation failed */
		sdldie("Unable to create window");

	checkSDLError(__LINE__);

	/* Create our opengl context and attach it to our window */
	maincontext = SDL_GL_CreateContext(mainwindow);
	checkSDLError(__LINE__);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	//glLoadIdentity(); //Reset the camera

	scene = aiImportFile("cube.obj", aiProcessPreset_TargetRealtime_MaxQuality);

	LoadArrays();

	chunkLoadDist = 32;
	//std::cin >> chunkLoadDist;

	GameLoop();
	aiReleaseImport(scene);
	OpenGLCleanup();

	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();

	return 0;
}