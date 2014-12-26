#pragma once

#include <core/oxygine.h>
#include <core/vertex.h>
#include <core/Texture.h>
#include <Polygon.h>

#include "LevelModel.h"


class LevelActor : public oxygine::Polygon {

private:
	struct ChunkUpdate {
		uint32_t x;
		uint32_t y;
		uint32_t width;
		uint32_t height;
	};

public:
	LevelActor(LevelModel* model);
	~LevelActor();

	void updateChunk(uint32_t x,  uint32_t y, uint32_t width, uint32_t height);
	void updateAll();

	// From Polygon->Sprite->Actor
	void doUpdate(const oxygine::UpdateState &us);
	void doRender(const oxygine::RenderState &rs);

private:
	std::vector<ChunkUpdate> _chunkUpdates;

	oxygine::vertexP2C* _vertices;
	
};