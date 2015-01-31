#pragma once

#include <glm/glm.hpp>
#include <Storage/GpuBuffer.h>

namespace Storage {
    namespace Geometry {
        extern void buildQuad(Storage::GpuBuffer<glm::vec3>* buffer, glm::vec2 pos, glm::vec2 size, float z);
    }
}