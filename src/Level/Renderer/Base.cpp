#include <Level/Renderer/Base.h>
#include <Level/Model/Base.h>

using namespace Level;
using namespace Level::Renderer;

Base::Base(Model::Base* model) {
	pAssocModel = model;
}

Base::~Base() {

}

// ----------------------------------

void Base::update() {

}

void Base::render() {

}
