#pragma once

#include <stdint.h>
#include <vector>
#include <map>

// Locals
#include <Level/Model/Chunk.h>
#include <Level/Renderer/Base.h>
#include <Level/Model/Layer.h>

namespace Level {
	namespace Model {

		class Base {

		public:
			const char* LevelName;

			uint32_t ChunksX; // Count of Chunks on X-Axis
			uint32_t ChunksY; // Count of Chunks on Y-Axis
			uint32_t ChunkScale; // width and height of one chunk

			//std::vector<Model::Chunk*> Chunks; // Chunks (column major allocated)
			std::vector<Model::Layer*> Layers;

			Level::Renderer::Base* Renderer;

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
			Model::Chunk* getChunkFromCoordinates(uint32_t x, uint32_t y);


		private:


		};

	}
}