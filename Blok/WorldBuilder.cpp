#include "WorldBuilder.h"
#include "World.h"

WorldBuilder* WorldBuilder::instance;

WorldBuilder::WorldBuilder()
{
	instance = this;
}


WorldBuilder::~WorldBuilder()
{
	delete blocks0;
	delete blocks1;
}

void WorldBuilder::Initialize()
{
	blocks0 = new GLfloat[4 * World::MAX_BLOCKS];
	blocks1 = new GLfloat[4 * World::MAX_BLOCKS];
}

void WorldBuilder::Update()
{
	if (builderDone && (frameCount == -1 || frameCount >= 6))
	{
		builderDone = false;
		frameCount = 0;

		builderThread = std::thread(&WorldBuilder::BuildList, this);

		if (blockList != -1)
			builderThread.detach();
	}

	frameCount++;
}

void WorldBuilder::GetBatch(GLfloat** blocks, GLuint* numBlocks)
{
	if (blockList == -1) // always wait for first batch
		builderThread.join();

	if (blockList == 0)
	{
		*blocks = WorldBuilder::instance->blocks0;
		*numBlocks = WorldBuilder::instance->numBlocks0;
	}

	else
	{
		*blocks = WorldBuilder::instance->blocks1;
		*numBlocks = WorldBuilder::instance->numBlocks1;
	}
}

void WorldBuilder::BuildList()
{
	GLfloat* blocks;
	GLuint* numBlocks;

	if (blockList == 0)
	{
		blocks = blocks1;
		numBlocks = &numBlocks1;
	}

	else if (blockList == -1 || 1)
	{
		blocks = blocks0;
		numBlocks = &numBlocks0;
	}

	int CHUNK_LOAD_DIST = 1;
	std::vector<std::thread> threads;

	glm::vec3 pos = Player::instance->GetPosition();

	for (int x = (pos.x - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x < (pos.x + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x++)
	{
		for (int z = (pos.z - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z < (pos.z + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z++)
		{
			if (!World::Instance->IsGenerated(x, z))
			{
				threads.push_back(std::thread(&World::GetBlocks, World::Instance, x, z));
			}
		}
	}

	for (std::vector<std::thread>::iterator t = threads.begin(); t != threads.end(); ++t)
	{
		t->join();
	}

	*numBlocks = 0;

	for (int x = (pos.x - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x < (pos.x + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; x++)
	{
		for (int z = (pos.z - (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z < (pos.z + (CHUNK_LOAD_DIST * World::SIDE)) / World::SIDE; z++)
		{
			AddChunkToList(x, z, blocks, numBlocks);
		}
	}

	if (blockList == -1 || blockList == 1)
		blockList = 0;
	else
		blockList = 1;

	builderDone = true;
}

void WorldBuilder::AddChunkToList(int x, int z, GLfloat* blocks, GLuint* numBlocks)
{
	int cX = x;
	int cZ = z;

	bool north = false;
	bool east = false;
	bool south = false;
	bool west = false;

	char*** northBA = nullptr;
	char*** eastBA = nullptr;
	char*** southBA = nullptr;
	char*** westBA = nullptr;

	if (!World::Instance->IsGenerated(cX, cZ))
		return;

	char*** blockArray = World::Instance->chunks[cX][cZ];

	if (World::Instance->IsGenerated(cX, cZ + 1))
	{
		north = true;
		northBA = World::Instance->chunks[cX][cZ + 1];
	}

	if (World::Instance->IsGenerated(cX + 1, cZ))
	{
		east = true;
		eastBA = World::Instance->chunks[cX + 1][cZ];
	}

	if (World::Instance->IsGenerated(cX, cZ - 1))
	{
		south = true;
		southBA = World::Instance->chunks[cX][cZ - 1];
	}

	if (World::Instance->IsGenerated(cX - 1, cZ))
	{
		west = true;
		westBA = World::Instance->chunks[cX - 1][cZ];
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
					blocks[*numBlocks * 4 + 0] = cX * World::SIDE + i;
					blocks[*numBlocks * 4 + 1] = k;
					blocks[*numBlocks * 4 + 2] = cZ * World::SIDE + j;
					blocks[*numBlocks * 4 + 3] = blockArray[i][j][k];

					*numBlocks += 1;
				}
			}
		}
	}
}