#pragma once

#include <glm/glm.hpp>

#include <Common/ModelAttribute.h>

namespace Control {
    class Camera {

    public:
        Camera();

        Common::ModelAttribute<unsigned int> X;
        Common::ModelAttribute<unsigned int> Y;


    };
}