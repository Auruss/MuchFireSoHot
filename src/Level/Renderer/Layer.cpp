#include <Level/Renderer/Layer.h>
#include <Level/Model/Layer.h>

#include <OpenGL/Global.h>

using namespace Level;
using namespace Level::Renderer;

Layer::Layer(Model::Layer* layer) {
	_model = layer;
    _buffer = OpenGL::Global::g_pLayerRenderSystem->requestCombined(4, 6);

    full_vertices_update();
}

// ------------------------------------------------------------------------

void Layer::update() {
	if (!_model->hasChanges()) return; // skip unnecessary ifs

	// check whether vertices update is required
	if(_model->X.hasChanged() || _model->Y.hasChanged() || _model->Scale.hasChanged()) {
        position_update();
	}

}

void Layer::render() {
	OpenGL::Global::g_pLayerRenderSystem->enqueueRenderJob(_buffer.index, 6);
}

// ------------------------------------------------------------------------

void Layer::full_vertices_update() {
    index_update();
    position_update();
    color_update();
}

void Layer::position_update() {
    OpenGL::Global::g_pPositionBuffer->beginUpdate(_buffer.vindex);

    glm::vec3* pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3(_model->X / 100.0f, _model->Y / 100.0f, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3(_model->X / 100.0f, (_model->Y - _model->Scale) / 100.0f, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((_model->X + _model->Scale) / 100.0f, (_model->Y - _model->Scale) / 100.0f, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((_model->X + _model->Scale) / 100.0f, _model->Y / 100.0f, 1.0f);

    OpenGL::Global::g_pPositionBuffer->endUpdate();
}
void Layer::color_update() {
    OpenGL::Global::g_pColorBuffer->beginUpdate(_buffer.vindex);

    glm::vec4* color = OpenGL::Global::g_pColorBuffer->next();
    *color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    OpenGL::Global::g_pColorBuffer->endUpdate();
}
void Layer::index_update() {
    OpenGL::Global::g_pIndexBuffer->beginUpdate(_buffer.index);
    *OpenGL::Global::g_pIndexBuffer->next() = 0 + _buffer.vindex;
    *OpenGL::Global::g_pIndexBuffer->next() = 1 + _buffer.vindex;
    *OpenGL::Global::g_pIndexBuffer->next() = 2 + _buffer.vindex;
    *OpenGL::Global::g_pIndexBuffer->next() = 2 + _buffer.vindex;
    *OpenGL::Global::g_pIndexBuffer->next() = 3 + _buffer.vindex;
    *OpenGL::Global::g_pIndexBuffer->next() = 0 + _buffer.vindex;
    OpenGL::Global::g_pIndexBuffer->endUpdate();
}