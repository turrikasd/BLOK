#include "Chunk.h"
#include <iostream>

Chunk::Chunk()
{
	pn = PerlinNoise(420);
	fn = FractalNoise();
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

	if (generated[cX][cZ] == NULL || !generated[cX][cZ] || y > Chunk::HEIGHT)
		return false;

	if (y < 0)
		return true;

	char*** blocks = chunks[cX][cZ];
	x = x % Chunk::SIDE;
	z = z % Chunk::SIDE;

	if (blocks[x][z][y] == 1)
		return true;

	return false;
}

void Chunk::PutBlockAt(int x, int y, int z, char type)
{
	int cX = x / SIDE;
	int cZ = z / SIDE;

	char*** blocks = chunks[cX][cZ];
	x = x % Chunk::SIDE;
	z = z % Chunk::SIDE;

	blocks[x][z][y] = type;

	//blocks[cX][cZ][bIndex]++;
}

bool Chunk::IsGenerated(int x, int z)
{
	return (!generated[x][z] == NULL && generated[x][z]);
		
}

void Chunk::GetBlocks(int x, int z)
{
	mutexes[x][z].lock();

	if (!IsGenerated(x, z))
		GenBlocks(x, z);

	mutexes[x][z].unlock();
}

void Chunk::GenBlocks(int x, int z)
{
	//std::cout << "Generating chunks.." << std::endl;

	int bX = x * Chunk::SIDE;
	int bZ = z * Chunk::SIDE;

	char*** blocks = new char**[SIDE];
	for (int xx = 0; xx < SIDE; xx++)
	{
		blocks[xx] = new char*[SIDE];
		for (int yy = 0; yy < SIDE; yy++)
		{
			blocks[xx][yy] = new char[HEIGHT];
		}
	}

	chunks[x][z] = &blocks[0];

	//double avg = 0;

	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
		{
			for (int k = 0; k < HEIGHT; k++)
			{
				double n = fn.Get(i + bX, j + bZ, k);
				//avg += n;

				blocks[i][j][k] = (n > (double)k / (double)HEIGHT) ? 1 : 0;
			}
		}
	}

	//avg = avg / SIDE / SIDE / HEIGHT;
	//std::cout << avg << std::endl;

	generated[x][z] = true;
}

/*void Chunk::GenBlocks(int x, int z)
{
	//std::cout << "Generating chunks.." << std::endl;

	int bX = x * Chunk::SIDE;
	int bZ = z * Chunk::SIDE;

	char*** blocks = new char**[SIDE];
	for (int xx = 0; xx < SIDE; xx++)
	{
		blocks[xx] = new char*[SIDE];
		for (int yy = 0; yy < SIDE; yy++)
		{
			blocks[xx][yy] = new char[HEIGHT];
		}
	}

	chunks[x][z] = &blocks[0];

	for (int i = 0; i < SIDE; i++)
	{
		for (int j = 0; j < SIDE; j++)
		{
			double n = pn.noise((double)(i + bX) / (double)64.0, (double)(j + bZ) / (double)64.0, 0.8);

			int y = (int)(n * 100);

			for (int k = 0; k < HEIGHT; k++)
			{
				blocks[i][j][k] = (y > k) ? 1 : 0;
			}

			//blocks[x][z][i * SIDE + j] = y;
			//blocks[PosToIndex(bX + i, bZ + j)] = y;
		}
	}

	generated[x][z] = true;
}*/
