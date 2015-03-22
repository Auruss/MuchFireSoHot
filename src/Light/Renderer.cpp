#include <Level/Editor.h>
#include <Light/Renderer.h>

using namespace Light;

Renderer::Renderer(Light::Model* owner) {
    _lightBulb.TextureCoord = glm::vec4(832.0f, 308.0f, 74.0f, 84.0f);
    _lightBulb.Rotation = 0;
    _lightBulb.Base = owner->Base;
    _lightBulb.Renderer = new Level::Renderer::Layer(&_lightBulb);

    Owner = owner;
}

void Renderer::update() {
    if(Level::Editor::Instance->isEditing()) {
        if(Owner->hasChanges()) {
            _lightBulb.X = Owner->X - Owner->Radius;
            _lightBulb.Y = Owner->Y - Owner->Radius;
            _lightBulb.Z = Owner->Z;
            _lightBulb.Width = Owner->Radius * 2.0f;
            _lightBulb.Height = Owner->Radius * 2.0f;
            _lightBulb.updateChanges();
            _lightBulb.Renderer->update();
        }

    }
}

void Renderer::render() {
    if(Level::Editor::Instance->isEditing()) {
        _lightBulb.Renderer->render();
    }
}