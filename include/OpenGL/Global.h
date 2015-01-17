#pragma once

#include <glm/glm.hpp>

#include <OpenGL/RenderSystem.h>
#include <Storage/GpuBuffer.h>

namespace OpenGL {
namespace Global {
	extern OpenGL::RenderSystem* g_pLayerRenderSystem;

	extern Storage::GpuBuffer<unsigned int>* g_pIndexBuffer;

	extern Storage::GpuBuffer<glm::vec3>* g_pPositionBuffer;
	extern Storage::GpuBuffer<glm::vec4>* g_pColorBuffer;

	extern void init();
	extern void update();
	extern void render();
}
}