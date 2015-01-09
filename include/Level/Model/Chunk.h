#pragma once

#include <stdint.h>
#include <vector>

#include <Level/Model/Block.h>

namespace Level {
	namespace Model {

		class Chunk {

		public:
			uint32_t ChunkX; // X position of chunk
			uint32_t ChunkY; // Y Position of chunk

			std::vector<Model::Block*> Blocks;
		};


	}
}
