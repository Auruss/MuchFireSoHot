#pragma once

#include <glm/glm.hpp>
#include <Storage/GpuBuffer.h>

namespace Storage {
    class GeometryBuilder {

    public:
        GeometryBuilder(Storage::GpuBuffer<glm::vec3>* buffer);

        void setRotation(float rot);
        void setRotationOrigin(glm::vec2);
        void setZOrder(float z);
        void setPosition(glm::vec2 pos);
        void setSize(glm::vec2 size);
        void setOffset(glm::vec2 offset);

        void setOriginToCenter();

        void buildQuad();

    private:
        Storage::GpuBuffer<glm::vec3>* _buffer;

        glm::vec2 _position;
        glm::vec2 _offset;
        glm::vec2 _size;
        float _rotation;
        glm::vec2 _origin;
        glm::mat4 _rot_matrix;
        float _zorder;
    };
}