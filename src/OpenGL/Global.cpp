#include <OpenGL/Global.h>
#include <OpenGL/Helper.h>

#include <Common/LiveLog/Builder.h>

#include <GL/glew.h>
#include <glm/ext.hpp>

#include <Common/LiveLog/CommonReflections.h>

OpenGL::RenderSystem* 		   OpenGL::Global::g_pLayerRenderSystem = NULL;

Storage::GpuBuffer<unsigned int>* OpenGL::Global::g_pIndexBuffer = NULL;

Storage::GpuBuffer<glm::vec3>* OpenGL::Global::g_pPositionBuffer = NULL;
Storage::GpuBuffer<glm::vec4>* OpenGL::Global::g_pColorBuffer = NULL;

Control::Camera* OpenGL::Global::g_pCamera = NULL;

int mModifier_Location = 0;
glm::mat4 mProjection;

void OpenGL::Global::init(int width, int height) {
	const char* vertex_shader = 
		"precision mediump float;"
		""
        "uniform mat4 mModifier;"
        ""
		"attribute vec3 vPosition;"
		"attribute vec4 vColor;"
		""
		"varying vec4 v_Color;"
		""
		"void main() {"
		"	v_Color = vColor;"
		"	gl_Position = mModifier * vec4(vPosition, 1.0);"
		"}";

	const char* frag_shader = 
		"precision mediump float;"
		""
		"varying vec4 v_Color;"
		""
		"void main() {"
		"	gl_FragColor = v_Color;"
		"}";

    // initialize camera
    g_pCamera = new Control::Camera();
    g_pCamera->X = 0;
    g_pCamera->Y = 0;


	// initialize global buffers
	g_pIndexBuffer    = new Storage::GpuBuffer<unsigned int>(1024, GL_ELEMENT_ARRAY_BUFFER);
	g_pPositionBuffer = new Storage::GpuBuffer<glm::vec3>(512, GL_ARRAY_BUFFER);
	g_pColorBuffer 	  = new Storage::GpuBuffer<glm::vec4>(512, GL_ARRAY_BUFFER);


	// initialize render system
    unsigned int program = OpenGL::Helper::createProgramFromMemory(vertex_shader, frag_shader);
	g_pLayerRenderSystem = new OpenGL::RenderSystem(program, g_pIndexBuffer);

    g_pLayerRenderSystem->addGpuBuffer("vPosition", g_pPositionBuffer);
    g_pLayerRenderSystem->addGpuBuffer("vColor", g_pColorBuffer);

    // uniforms
    mModifier_Location = glGetUniformLocation(program, "mModifier");
    mProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);

    Common::LiveLog::Builder builder(LOG_PROGRAM_MVP);
    builder.addRefObj("mProjection", &__mat4_Reflection, (float*)&mProjection);
    builder.push();
}

void OpenGL::Global::update() {
    if(g_pCamera->X.hasChanged() || g_pCamera->Y.hasChanged()) {
        glm::mat4 view = glm::translate(glm::vec3(-(float)g_pCamera->X, (float)g_pCamera->Y, -100.0f));

        glUseProgram(g_pLayerRenderSystem->getProgram());
        glm::mat4 mvp = mProjection * view;
        glUniformMatrix4fv(mModifier_Location, 1, GL_FALSE, (float*)&mvp);
    }

	g_pIndexBuffer->uploadChanges();
	g_pPositionBuffer->uploadChanges();
	g_pColorBuffer->uploadChanges();
}

void OpenGL::Global::render() {
	g_pLayerRenderSystem->render();
}