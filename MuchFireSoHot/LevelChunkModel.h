#pragma once

#include <stdint.h>
#include <vector>

#include "LevelBlockModel.h"

class LevelChunkModel {

public:
	uint32_t ChunkX; // X position of chunk
	uint32_t ChunkY; // Y Position of chunk

	std::vector<LevelBlockModel*> Blocks;
};