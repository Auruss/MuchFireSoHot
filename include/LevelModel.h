#pragma once

#include <stdint.h>
#include <vector>
#include <map>


// Locals
#include "LevelChunkModel.h"
#include "LevelBackgroundModel.h"
#include "LevelTrigger.h"

class LevelModel {

public:
	const char* LevelName;

	uint32_t ChunksX; // Count of Chunks on X-Axis
	uint32_t ChunksY; // Count of Chunks on Y-Axis
	uint32_t ChunkScale; // width and height of one chunk

	std::vector<LevelChunkModel*> Chunks; // Chunks (column major allocated)
	
	
	LevelBackgroundModel* Background; // Background layer


	/**
		Calculates the amount of required local memory.

		@returns memory in bytes
	*/
	uint64_t getLocalMemoryRequired();

	
	/**
		Gets ChunkModel from coordinates

		@param x X
		@param y Y

		@returns Pointer to ChunkModel staying at (x,y)
	*/
	LevelChunkModel* getChunkFromCoordinates(uint32_t x, uint32_t y);


private:


};