#include <Level/Renderer/Layer.h>
#include <Level/Model/Layer.h>

#include <OpenGL/Global.h>
#include <Storage/Geometry.h>
#include <Level/Model/Base.h>

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
    if(_model->TextureCoord.hasChanged()) {
        texcoord_update();
    }
}

void Layer::render() {
	OpenGL::Global::g_pLayerRenderSystem->enqueueRenderJob(_buffer.index, 6);
}

// ------------------------------------------------------------------------

void Layer::full_vertices_update() {
    index_update();
    position_update();
    texcoord_update();
}

void Layer::position_update() {
    OpenGL::Global::g_pPositionBuffer->beginUpdate(_buffer.vindex);

    Storage::Geometry::buildQuad(OpenGL::Global::g_pPositionBuffer,
            glm::vec2((float)_model->X, (float)_model->Y),
            glm::vec2((float)_model->Width, (float)_model->Height), _model->Z);

    OpenGL::Global::g_pPositionBuffer->endUpdate();
}

void Layer::texcoord_update() {
    auto texture = _model->Base->Renderer->TextureSize;

    auto buffer = OpenGL::Global::g_pTexCoordBuffer;
    buffer->beginUpdate(_buffer.vindex);

    auto coord = buffer->next();
    coord->x = _model->TextureCoord->x / texture.x;
    coord->y = _model->TextureCoord->y / texture.y;

    coord = buffer->next();
    coord->x = _model->TextureCoord->x / texture.x;
    coord->y = (_model->TextureCoord->y + _model->TextureCoord->w) / texture.y;

    coord = buffer->next();
    coord->x = (_model->TextureCoord->x + _model->TextureCoord->z) / texture.x;
    coord->y = (_model->TextureCoord->y + _model->TextureCoord->w) / texture.y;

    coord = buffer->next();
    coord->x = (_model->TextureCoord->x + _model->TextureCoord->z) / texture.x;
    coord->y = _model->TextureCoord->y / texture.y;

    buffer->endUpdate();
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