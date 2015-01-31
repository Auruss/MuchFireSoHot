#include <Storage/Geometry.h>

void Storage::Geometry::buildQuad(Storage::GpuBuffer<glm::vec3>* buffer, glm::vec2 pos, glm::vec2 size, float z) {
    z = 100 - z;

    glm::vec3* vec = buffer->next();
    *vec = glm::vec3(pos.x, pos.y, z);

    vec = buffer->next();
    *vec = glm::vec3(pos.x, pos.y + size.y, z);

    vec = buffer->next();
    *vec = glm::vec3(pos.x + size.x, pos.y + size.y, z);

    vec = buffer->next();
    *vec = glm::vec3(pos.x + size.x, pos.y, z);
}