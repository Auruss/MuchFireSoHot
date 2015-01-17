#include <Level/Renderer/Layer.h>
#include <Level/Model/Layer.h>

#include <OpenGL/Global.h>

using namespace Level;
using namespace Level::Renderer;

Layer::Layer(Model::Layer* layer) {
	_model = layer;
	_buffer_index = OpenGL::Global::g_pIndexBuffer->request(6);
	OpenGL::Global::g_pPositionBuffer->request(4);
	OpenGL::Global::g_pColorBuffer->request(4);
}

// ------------------------------------------------------------------------

void Layer::update() {
	if (!_model->hasChanges()) return; // skip unnecessary ifs

	// check whether vertices update is required
	if(_model->X.hasChanged() || _model->Y.hasChanged() || _model->Scale.hasChanged()) {
		full_vertices_update();
	}
}

void Layer::render() {
	OpenGL::Global::g_pLayerRenderSystem->enqueueRenderJob(0, 6);
}

// ------------------------------------------------------------------------

void Layer::full_vertices_update() {
	OpenGL::Global::g_pIndexBuffer->beginUpdate(_buffer_index);
	OpenGL::Global::g_pPositionBuffer->beginUpdate(_buffer_index);
	OpenGL::Global::g_pColorBuffer->beginUpdate(_buffer_index);

	// vertices
	glm::vec3* pos;
	glm::vec4* color;


	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(_model->X / 100.0f, _model->Y / 100.0f, 1.0f);
	*color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(_model->X / 100.0f, (_model->Y - _model->Scale) / 100.0f, 1.0f);
	*color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3((_model->X + _model->Scale) / 100.0f, (_model->Y - _model->Scale) / 100.0f, 1.0f);
	*color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3((_model->X + _model->Scale) / 100.0f, _model->Y / 100.0f, 1.0f);
	*color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	/*pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(0.0f, 0.0f, 1.0f);
	*color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(0.0f, -1.0f, 1.0f);
	*color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(1.0f, -1.0f, 1.0f);
	*color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	pos = OpenGL::Global::g_pPositionBuffer->next();
	color = OpenGL::Global::g_pColorBuffer->next();
	*pos = glm::vec3(1.0f, 0.0f, 1.0f);
	*color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);*/

	// indices
	*OpenGL::Global::g_pIndexBuffer->next() = 0;
	*OpenGL::Global::g_pIndexBuffer->next() = 1;
	*OpenGL::Global::g_pIndexBuffer->next() = 2;
	*OpenGL::Global::g_pIndexBuffer->next() = 2;
	*OpenGL::Global::g_pIndexBuffer->next() = 3;
	*OpenGL::Global::g_pIndexBuffer->next() = 0;

	OpenGL::Global::g_pIndexBuffer->endUpdate();
	OpenGL::Global::g_pPositionBuffer->endUpdate();
	OpenGL::Global::g_pColorBuffer->endUpdate();
}