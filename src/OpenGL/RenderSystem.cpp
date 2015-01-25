#include <cstdio>

#include <OpenGL/RenderSystem.h>
#include <Common/GameTime.h>
#include <Common/LiveLog/Builder.h>

#include <GL/glew.h>


using namespace OpenGL;

static Common::LiveLog::ReflObject __LogStatsDrawCallsRefl;
static bool __ReflectionImpl = false;


RenderSystem::RenderSystem(unsigned int index_buffer, unsigned int program) {
    if(!__ReflectionImpl) {
        __LogStatsDrawCallsRefl.init<LogStatsDrawCalls>();
        __LogStatsDrawCallsRefl.addMember<int>("draw_calls", offsetof(LogStatsDrawCalls,draw_calls));
        __LogStatsDrawCallsRefl.addMember<int>("faces", offsetof(LogStatsDrawCalls,faces));
        __ReflectionImpl = true;
    }

	_program = program;
	_index_buffer = index_buffer;
    _stats.draw_calls = 0;
    _stats.faces = 0;
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

// ---------------------------------------------


void RenderSystem::defragedRender() {
	bindBuffers();
	
	_render_jobs.clear();
}

void RenderSystem::fragedRender() {
	bindBuffers();

	for (auto iter = _render_jobs.begin(); iter != _render_jobs.end(); iter++) {
		glDrawElements(GL_TRIANGLES, iter->count, GL_UNSIGNED_INT, (GLvoid*)iter->offset);
        _stats.draw_calls++;
        _stats.faces += iter->count / 3;
	}

	if(Common::GameTime::tickEvery(500, _stats_timer, false)) {
        Common::LiveLog::Builder builder(LOG_STATS_DRAW_CALLS);
        builder.addRefObj("stats", &__LogStatsDrawCallsRefl, (void*)&_stats);
        builder.push();
        _stats.faces = 0;
        _stats.draw_calls = 0;
	}

	_render_jobs.clear();
}