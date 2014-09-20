#include "Chunk.h"
#include <iostream>

Chunk::Chunk()
{
	pn = PerlinNoise(420);
}


Chunk::~Chunk()
{
}

int Chunk::PosToIndex(int x, int z)
{
	return Chunk::SIDE * x + z;
}

void Chunk::IndexToPos(int index, int* x, int* z)
{
	*x = index / Chunk::SIDE;
	*z = index % Chunk::SIDE;
}

bool Chunk::IsBlockAt(int x, int y, int z)
{
	int cX = x / SIDE;
	int cZ = z / SIDE;

	int bIndex = PosToIndex(x % SIDE, z % SIDE);

	if (blocks[cX][cZ][bIndex] >= y)
		return true;

	return false;
}

void Chunk::PutBlockAt(int x, int y, int z)
{
	int cX = x / SIDE;
	int cZ = z / SIDE;

	int bIndex = PosToIndex(x % SIDE, z % SIDE);

	blocks[cX][cZ][bIndex]++;
}

int Chunk::GetBlocks(int x, int z)
{
	if (generated[x][z] == NULL || !generated[x][z])
		GenBlocks(x, z);

	return PosToIndex(x * Chunk::SIDE, z * Chunk::SIDE);
}

void Chunk::GenBlocks(int x, int z)
{
	//std::cout << "Generating chunks.." << std::endl;

	int bX = x * Chunk::SIDE;
	int bZ = z * Chunk::SIDE;

	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
		{
			double n = pn.noise((double)(i + bX) / (double)64.0, (double)(j + bZ) / (double)64.0, 0.8);

			int y = n * 100;
			//y -= 50;

			blocks[x][z][i * SIDE + j] = y;
			//blocks[PosToIndex(bX + i, bZ + j)] = y;
		}
	}

	generated[x][z] = true;
}
