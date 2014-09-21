#pragma once

#include "glm\glm.hpp"
#include "PerlinNoise.h"

class Chunk
{
public:
	static const int WORLD_SIZE = 8192;//8192;
	static const int HALF_WORLD = WORLD_SIZE / 2;
	static const int SIDE = 32;
	static const int HEIGHT = 256;
	static const int CHUNKS = WORLD_SIZE / SIDE;
	static const int MAX_BLOCKS = 2097152;// 65536;

	static int PosToIndex(int x, int z);
	static void IndexToPos(int index, int *x, int * z);

	Chunk();
	~Chunk();

	Chunk *Instance;

	bool IsBlockAt(int x, int y, int z);
	void PutBlockAt(int x, int y, int z, char type);

	void GenBlocks(int x, int z);
	int GetBlocks(int x, int z);
	char*** chunks[CHUNKS][CHUNKS];

private:
	PerlinNoise pn;
	bool generated[CHUNKS][CHUNKS];
};

