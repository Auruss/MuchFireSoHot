#include <Level/Renderer/Layer.h>
#include <Level/Model/Layer.h>

#include <OpenGL/Global.h>
#include <Storage/Geometry.h>

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

	// position update
	if(_model->X.hasChanged() || _model->Y.hasChanged() || _model->Z.hasChanged()
            || _model->Width.hasChanged() || _model->Height.hasChanged())
    {
        position_update();
	}

    // color update
    if(_model->Color.hasChanged()) {
        color_update();
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

    /*glm::vec3* pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((float)_model->X, (float)_model->Y, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((float)_model->X, (float)_model->Y + _model->Height, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((float)_model->X + _model->Width, (float)_model->Y + _model->Height, 1.0f);

    pos = OpenGL::Global::g_pPositionBuffer->next();
    *pos = glm::vec3((float)_model->X + _model->Width, (float)_model->Y, 1.0f);*/
    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2((float)_model->X, (float)_model->Y),
            glm::vec2((float)_model->Width, (float)_model->Height), _model->Z);

    OpenGL::Global::g_pPositionBuffer->endUpdate();
}
void Layer::color_update() {
    OpenGL::Global::g_pColorBuffer->beginUpdate(_buffer.vindex);

    glm::vec4* color = OpenGL::Global::g_pColorBuffer->next();
    *color = _model->Color;

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = _model->Color;

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = _model->Color;

    color = OpenGL::Global::g_pColorBuffer->next();
    *color = _model->Color;

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