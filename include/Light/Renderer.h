#pragma once

#include <Light/Model.h>
#include <Level/Model/Layer.h>

namespace Light {
    class Renderer {

    public:
        Renderer(Light::Model* owner);

        void update();
        void render();

        Light::Model* Owner;

    private:
        Level::Model::Layer _lightBulb;
        //Level::Model::Layer _lightMesh;

    };
}