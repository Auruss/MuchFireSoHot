#include <cstdio>

#include <OpenGL/RenderSystem.h>
#include <Common/GameTime.h>

#include <GL/glew.h>


using namespace OpenGL;

RenderSystem::RenderSystem(unsigned int index_buffer, unsigned int program) {
	_program = program;
	_index_buffer = index_buffer;
}

// --------------------------------------------

void RenderSystem::addBuffer(unsigned int gl, char* shader_param_name, unsigned int type, int count) {
	buffer_settings set;
	set.id = gl;
	set.location = glGetAttribLocation(_program, shader_param_name);
	set.type = type;
	set.count = count;

	_buffers.push_back(set);
}


void RenderSystem::enqueueRenderJob(unsigned int offset, unsigned int count) {
	render_job job;
	job.count = count;
	job.offset = offset;
	_render_jobs.push_back(job);
}

// ---------------------------------------------

void RenderSystem::bindBuffers() {
	glUseProgram(_program);

	for (auto iter = _buffers.begin(); iter != _buffers.end(); iter++) {
		glBindBuffer(GL_ARRAY_BUFFER, iter->id);
		glEnableVertexAttribArray(iter->location);
		glVertexAttribPointer(iter->location, iter->count, iter->type, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
}

void RenderSystem::defragedRender() {
	bindBuffers();
	
	_render_jobs.clear();
}

void RenderSystem::fragedRender() {
	static int call_stats = 0;
	static GameTimeObj stat;

	bindBuffers();

	for (auto iter = _render_jobs.begin(); iter != _render_jobs.end(); iter++) {
		glDrawElements(GL_TRIANGLES, iter->count, GL_UNSIGNED_INT, (GLvoid*)iter->offset);
		call_stats++;
	}

	if(Common::GameTime::tickEvery(10000, stat, false)) {
		printf("%d draw calls in 10 seconds. \n", call_stats);
		call_stats = 0;
	}

	_render_jobs.clear();
}