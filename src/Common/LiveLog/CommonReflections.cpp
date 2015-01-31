#include <Common/LiveLog/CommonReflections.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Common::LiveLog::ReflObject __vec4_Reflection;
Common::LiveLog::ReflObject __mat4_Reflection;

void Common::LiveLog::init_common_reflections() {
    // glm::vec4
    __vec4_Reflection.init<glm::vec4>();
    __vec4_Reflection.addMember<float>("x", offsetof(glm::vec4, x));
    __vec4_Reflection.addMember<float>("y", offsetof(glm::vec4, y));
    __vec4_Reflection.addMember<float>("z", offsetof(glm::vec4, z));
    __vec4_Reflection.addMember<float>("w", offsetof(glm::vec4, w));
    if(__vec4_Reflection.sizeOf() != sizeof(glm::vec4)) {
        fprintf(stderr, "[Reflection] sizeof check failed for glm::vec4\n");
    }

    // glm::mat4
    __mat4_Reflection.init<glm::mat4>();
    __mat4_Reflection.addOtherReflection("0", 0, &__vec4_Reflection);
    __mat4_Reflection.addOtherReflection("1", __vec4_Reflection.sizeOf(), &__vec4_Reflection);
    __mat4_Reflection.addOtherReflection("2", __vec4_Reflection.sizeOf() * 2, &__vec4_Reflection);
    __mat4_Reflection.addOtherReflection("3", __vec4_Reflection.sizeOf() * 3, &__vec4_Reflection);
    if(__mat4_Reflection.sizeOf() != sizeof(glm::mat4)) {
        fprintf(stderr, "[Reflection] sizeof check failed for glm::mat4\n");
    }
}