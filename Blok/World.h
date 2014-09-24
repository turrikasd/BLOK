#pragma once

#include "glm\glm.hpp"
#include "FractalNoise.h"
#include "WorldBuilder.h"
#include "Player.h"
#include <mutex>
#include <vector>

class World
{
public:
	static const int WORLD_SIZE = 8192;//8192;
	static const int HALF_WORLD = WORLD_SIZE / 2;
	static const int SIDE = 32;
	static const int HEIGHT = 128;
	static const int CHUNKS = WORLD_SIZE / SIDE;
	static const int MAX_BLOCKS = 2097152;// 65536;

	World();
	~World();

	void Initialize();
	void UpdateAll(float deltaTime);
	static World *Instance;

	bool IsBlockAt(float x, float y, float z);
	bool IsBlockAt(glm::vec3 pos);
	bool IsBlockAt(int x, int y, int z);
	void PutBlockAt(int x, int y, int z, char type);

	void GenBlocks(int x, int z);
	bool IsGenerated(int x, int z);
	void GetBlocks(int x, int z);
	char*** chunks[CHUNKS][CHUNKS];

private:
	FractalNoise fn;
	std::mutex mutexes[CHUNKS][CHUNKS];
	bool generated[CHUNKS][CHUNKS];

	Player* player;

	WorldBuilder* worldBuilder;
};