#pragma once

#include "GL\glew.h"
#include <thread>

class WorldBuilder
{
public:
	WorldBuilder();
	~WorldBuilder();

	static WorldBuilder* instance;
	void Initialize();

	void Update();
	void GetBatch(GLfloat** blocks, GLuint* numBlocks);
	void AddChunkToList(int x, int z, GLfloat* blocks, GLuint* numBlocks);

private:
	void BuildList();

private:
	bool builderDone = true;
	int frameCount = -1;
	int blockList = -1;
	std::thread builderThread;

public:
	GLfloat *blocks0;
	GLfloat *blocks1;

	GLuint numBlocks0 = 0;
	GLuint numBlocks1 = 0;
};

