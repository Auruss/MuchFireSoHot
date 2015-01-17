#include <OpenGL/Global.h>
#include <OpenGL/Helper.h>

#include <GL/glew.h>


OpenGL::RenderSystem* 		   OpenGL::Global::g_pLayerRenderSystem = NULL;

Storage::GpuBuffer<unsigned int>* OpenGL::Global::g_pIndexBuffer = NULL;

Storage::GpuBuffer<glm::vec3>* OpenGL::Global::g_pPositionBuffer = NULL;
Storage::GpuBuffer<glm::vec4>* OpenGL::Global::g_pColorBuffer = NULL;

void OpenGL::Global::init() {
	const char* vertex_shader = 
		"precision mediump float;"
		""
		"attribute vec3 vPosition;"
		"attribute vec4 vColor;"
		""
		"varying vec4 v_Color;"
		""
		"void main() {"
		"	v_Color = vColor;"
		"	gl_Position = vec4(vPosition, 1.0);"
		"}";

	const char* frag_shader = 
		"precision mediump float;"
		""
		"varying vec4 v_Color;"
		""
		"void main() {"
		"	gl_FragColor = v_Color;"
		"}";

	// initialize global buffers
	g_pIndexBuffer    = new Storage::GpuBuffer<unsigned int>(1024, GL_ELEMENT_ARRAY_BUFFER);
	g_pPositionBuffer = new Storage::GpuBuffer<glm::vec3>(512, GL_ARRAY_BUFFER);
	g_pColorBuffer 	  = new Storage::GpuBuffer<glm::vec4>(512, GL_ARRAY_BUFFER);


	// initialize render system
	g_pLayerRenderSystem = new OpenGL::RenderSystem(g_pIndexBuffer->getInternId(),
			OpenGL::Helper::createProgramFromMemory(vertex_shader, frag_shader));
	g_pLayerRenderSystem->addBuffer(g_pPositionBuffer->getInternId(), (char*)"vPosition", GL_FLOAT, 3);
	g_pLayerRenderSystem->addBuffer(g_pColorBuffer->getInternId(), (char*)"vColor", GL_FLOAT, 4);
}

void OpenGL::Global::update() {
	g_pIndexBuffer->uploadChanges();
	g_pPositionBuffer->uploadChanges();
	g_pColorBuffer->uploadChanges();
}

void OpenGL::Global::render() {
	g_pLayerRenderSystem->fragedRender(); // TODO: Do defraged rendering
}