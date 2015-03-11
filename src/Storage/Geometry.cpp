#include <Storage/Geometry.h>
#include <glm/gtx/rotate_vector.hpp>

using namespace Storage;

GeometryBuilder::GeometryBuilder(Storage::GpuBuffer<glm::vec3> *buffer) {
    _buffer = buffer;
}

// ======================================================================================

void GeometryBuilder::setPosition(glm::vec2 pos) {
    _position = pos;
}

void GeometryBuilder::setSize(glm::vec2 size) {
    _size = size;
}

void GeometryBuilder::setZOrder(float z) {
    _zorder = z;
}

void GeometryBuilder::setRotationOrigin(glm::vec2 origin) {
    _origin = origin;
}

void GeometryBuilder::setOriginToCenter() {
    _origin = _position + _size / 2.0f;
}

void GeometryBuilder::setRotation(float rot) {
    _rotation = rot;
    _rot_matrix = glm::rotate(glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
}

// ======================================================================================

void GeometryBuilder::buildQuad() {
    float z = 100 - _zorder;

    auto rotate = [this, z] (glm::vec3& vec) {
        if(_rotation == 0) return;

        // translate to origin
        vec.x -= _origin.x;
        vec.y -= _origin.y;

        // rotate
        auto temp = _rot_matrix * glm::vec4(vec, 1.0f);
        vec = temp.xyz() / temp.w;
        vec.z = z;

        // translate back
        vec.x += _origin.x;
        vec.y += _origin.y;
    };

    glm::vec3* vec = _buffer->next();
    *vec = glm::vec3(_position.x, _position.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x, _position.y + _size.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x + _size.x, _position.y + _size.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x + _size.x, _position.y, z);
    rotate(*vec);
}
