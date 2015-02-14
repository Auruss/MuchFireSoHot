#pragma once

#include <stdint.h>

namespace OpenGL {
    namespace StateCache {

        void bindVertexBuffer(uint32_t id);
        void bindIndexBuffer(uint32_t id);

        bool useProgram(uint32_t id);

        void enableAttributes(uint32_t count);
    }
}