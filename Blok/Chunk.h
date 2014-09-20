#pragma once

#include "glm\glm.hpp"
#include "PerlinNoise.h"

class Chunk
{
public:
	static const int WORLD_SIZE = 8192;
	static const int MAX_BLOCKS = 65536;//131072;
	static const int HALF_WORLD = WORLD_SIZE / 2;
	static const int SIDE = 2;
	static const int CHUNKS = WORLD_SIZE / SIDE;

	static int PosToIndex(int x, int z);
	static void IndexToPos(int index, int *x, int * z);

	Chunk();
	~Chunk();

	Chunk *Instance;

	bool IsBlockAt(int x, int y, int z);
	void PutBlockAt(int x, int y, int z);

	void GenBlocks(int x, int z);
	int GetBlocks(int x, int z);
	int blocks[CHUNKS][CHUNKS][SIDE * SIDE];

private:
	PerlinNoise pn;
	bool generated[CHUNKS][CHUNKS];
};

