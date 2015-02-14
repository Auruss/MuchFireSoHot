#include <OpenGL/StateCache.h>

#include <GL/glew.h>

using namespace OpenGL;

uint32_t _vbuffer = -1;
uint32_t _ibuffer = -1;
uint32_t _program = -1;
uint32_t _attributes = 0;

// ===================================================================================

void StateCache::bindVertexBuffer(uint32_t id) {
    if(_vbuffer != id) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        _vbuffer = id;
    }
}

void StateCache::bindIndexBuffer(uint32_t id) {
    if(_ibuffer != id) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        _ibuffer = id;
    }
}

bool StateCache::useProgram(uint32_t id) {
    if(_program != id) {
        glUseProgram(id);
        _program = id;
        return true;
    }
    return false;
}

// ===================================================================================

void StateCache::enableAttributes(uint32_t count) {
    if(count > _attributes) {
        int loops = count - _attributes;
        for(int i = 0; i < loops; i++) {
            glEnableVertexAttribArray(i);
        }
    } else {
        for(int i = count; i < _attributes; i++) {
            glDisableVertexAttribArray(i);
        }
    }
}

// ===================================================================================