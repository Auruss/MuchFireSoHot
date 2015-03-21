#pragma once

#include <Common/BaseModel.h>
#include <Common/ModelAttribute.h>
#include <Level/Model/Base.h>

namespace Light {
    class Renderer;

    class Model : public Common::BaseModel {

    public:
        Common::ModelAttribute<int> X;
        Common::ModelAttribute<int> Y;
        Common::ModelAttribute<int> Z;

        Common::ModelAttribute<int> Radius;
        Common::ModelAttribute<float> Strength;

        Common::ModelAttribute<bool> HasMesh;

        Light::Renderer* Renderer;
        Level::Model::Base* Base;

    };
};