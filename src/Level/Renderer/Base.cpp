#include <Level/Renderer/Base.h>
#include <Level/Model/Base.h>
#include <Light/Renderer.h>

using namespace Level;
using namespace Level::Renderer;

Base::Base(Model::Base* model) {
	pAssocModel = model;
}

Base::~Base() {

}

// ----------------------------------

void Base::update() {
	// update lights
	for(auto iter = pAssocModel->Lights.begin(); iter != pAssocModel->Lights.end(); iter++) {
		(*iter)->Renderer->update();
	}

	// update layers
	for(auto iter = pAssocModel->Layers.begin(); iter != pAssocModel->Layers.end(); iter++) {
		(*iter)->Renderer->update();
	}
}

void Base::render() {
	// render lights
	for(auto iter = pAssocModel->Lights.begin(); iter != pAssocModel->Lights.end(); iter++) {
		(*iter)->Renderer->render();
	}

	// render layers
	for(auto iter = pAssocModel->Layers.begin(); iter != pAssocModel->Layers.end(); iter++) {
		(*iter)->Renderer->render();
	}
}
