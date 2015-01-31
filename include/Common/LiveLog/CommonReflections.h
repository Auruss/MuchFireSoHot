#pragma once

#include <Common/LiveLog/Reflection.h>

extern Common::LiveLog::ReflObject __vec4_Reflection;
extern Common::LiveLog::ReflObject __mat4_Reflection;

namespace Common {
    namespace LiveLog {
        void init_common_reflections();
    }
}