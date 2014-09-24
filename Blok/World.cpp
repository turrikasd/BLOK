#include "World.h"
#include <iostream>

World *World::Instance;

World::World()
{
	Instance = this;
	fn = FractalNoise();
}

World::~World()
{
	delete worldBuilder;
	delete player;
}

void World::Initialize()
{
	player = new Player();
	worldBuilder = new WorldBuilder();
	worldBuilder->Initialize();
}

void World::UpdateAll(float deltaTime)
{
	worldBuilder->Update();

	for (std::vector<Actor*>::iterator it = Actor::actors.begin(); it != Actor::actors.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

bool World::IsBlockAt(glm::vec3 pos)
{
	return IsBlockAt(pos.x, pos.y, pos.z);
}

bool World::IsBlockAt(float x, float y, float z)
{
	return IsBlockAt((int)(x + 0.5f), (int)(y + 0.5f), (int)(z + 0.5f));
}

bool World::IsBlockAt(int x, int y, int z)
{
	int cX = x / SIDE;
	int cZ = z / SIDE;

	if (y > World::HEIGHT)
		return false;

	if (y < 0)
		return true;

	if (generated[cX][cZ] == NULL || !generated[cX][cZ])
		return false;

	char*** blocks = chunks[cX][cZ];
	x = x % World::SIDE;
	z = z % World::SIDE;

	if (blocks[x][z][y] != 1)
		return true;

	return false;
}

void World::PutBlockAt(int x, int y, int z, char type)
{
	int cX = x / SIDE;
	int cZ = z / SIDE;

	char*** blocks = chunks[cX][cZ];
	x = x % World::SIDE;
	z = z % World::SIDE;

	blocks[x][z][y] = type;
}

bool World::IsGenerated(int x, int z)
{
	return (!generated[x][z] == NULL && generated[x][z]);
}

void World::GetBlocks(int x, int z)
{
	mutexes[x][z].lock();

	if (!IsGenerated(x, z))
		GenBlocks(x, z);

	mutexes[x][z].unlock();
}

void World::GenBlocks(int x, int z)
{
	int bX = x * World::SIDE;
	int bZ = z * World::SIDE;

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
			for (int k = 0; k < HEIGHT; k++)
			{
				double n = fn.Get(i + bX, k, j + bZ);

				blocks[i][j][k] = (char)n;
			}
		}
	}

	generated[x][z] = true;
}
