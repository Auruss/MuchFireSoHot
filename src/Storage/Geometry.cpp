#include <Storage/Geometry.h>
#include <glm/gtx/rotate_vector.hpp>

using namespace Storage;

GeometryBuilder::GeometryBuilder(Storage::GpuBuffer<glm::vec3> *buffer) {
    _buffer = buffer;
    _offset = glm::vec2(0.0f, 0.0f);
}

// ======================================================================================

void GeometryBuilder::setPosition(glm::vec2 pos) {
    _position = pos;
}

void GeometryBuilder::setSize(glm::vec2 size) {
    _size = size;
}

void GeometryBuilder::setOffset(glm::vec2 offset) {
    _offset = offset;
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
        vec.x -= _origin.x + _offset.x;
        vec.y -= _origin.y + _offset.y;

        // rotate
        auto temp = _rot_matrix * glm::vec4(vec, 1.0f);
        vec = temp.xyz() / temp.w;
        vec.z = z;

        // translate back
        vec.x += _origin.x + _offset.x;
        vec.y += _origin.y + _offset.y;
    };

    glm::vec3* vec = _buffer->next();
    *vec = glm::vec3(_position.x + _offset.x, _position.y + _offset.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x + _offset.x, _position.y + _size.y + _offset.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x + _size.x + _offset.x, _position.y + _size.y + _offset.y, z);
    rotate(*vec);

    vec = _buffer->next();
    *vec = glm::vec3(_position.x + _size.x + _offset.x, _position.y + _offset.y, z);
    rotate(*vec);
}
