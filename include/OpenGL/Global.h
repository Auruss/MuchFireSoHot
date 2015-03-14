#pragma once

#include <glm/glm.hpp>

#include <OpenGL/RenderSystem.h>
#include <Storage/GpuBuffer.h>
#include <Control/Camera.h>

namespace OpenGL {
namespace Global {
	extern OpenGL::RenderSystem* g_pLayerRenderSystem;

	extern Storage::GpuBuffer<unsigned int>* g_pIndexBuffer;

	extern Storage::GpuBuffer<glm::vec3>* g_pPositionBuffer;
    extern Storage::GpuBuffer<glm::vec2>* g_pTexCoordBuffer;
	extern Storage::GpuBuffer<glm::vec2>* g_pOriginBuffer;

    extern Control::Camera* g_pCamera;

	extern int g_Width;
	extern int g_Height;

	extern glm::mat4* getModifierMatrix();

	extern void init(int width, int height);
	extern void update();
	extern void render();
}
}