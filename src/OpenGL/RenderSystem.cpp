#include <cstdio>

#include <OpenGL/StateCache.h>
#include <OpenGL/RenderSystem.h>
#include <Common/GameTime.h>
#include <Common/LiveLog/Builder.h>


using namespace OpenGL;

static Common::LiveLog::ReflObject __LogStatsDrawCallsRefl;
static bool __ReflectionImpl = false;


RenderSystem::RenderSystem(unsigned int program, Storage::GpuBuffer<unsigned int>* index_buffer) {
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


void RenderSystem::enqueueRenderJob(unsigned int offset, unsigned int count) {
	_render_jobs.addFragment(offset, count);
}

// ---------------------------------------------

RenderSystem::Combined RenderSystem::requestCombined(int vertices, int indices) {
    int index = _index_buffer->request(indices);

    int vindex = 0;
    for(auto iter = _buffers.begin(); iter != _buffers.end(); iter++) {
        if(iter == _buffers.begin()) {
            vindex = iter->owner->request(vertices);
            continue;
        }
        if(vindex != iter->owner->request(vertices)) {
            Common::LiveLog::Builder builder(LOG_CRITICAL_RENDER_ERROR);
            builder.setMessage("[RenderSystem] Buffers are out of sync");
            builder.push();
            return RenderSystem::Combined();
        }
    }

    RenderSystem::Combined res;
    res.index = index;
    res.vindex = vindex;
    return res;
}

// ---------------------------------------------

unsigned int RenderSystem::attribLocation(const char* eq) {
    return (unsigned int)glGetAttribLocation(_program, eq);
}

void RenderSystem::bindBuffers() {
    if(StateCache::useProgram(_program)) {
        StateCache::enableAttributes(_buffers.size());
        for (auto iter = _buffers.begin(); iter != _buffers.end(); iter++) {
            StateCache::bindVertexBuffer(iter->owner->getInternId());
            glVertexAttribPointer(iter->location, iter->count, iter->type, GL_FALSE, 0, 0);
        }
    }

    StateCache::bindIndexBuffer(_index_buffer->getInternId());
}

// ---------------------------------------------


void RenderSystem::render() {
	bindBuffers();

    unsigned int count = 0;
    auto vector = _render_jobs.defrag(&count);

	for (auto iter = vector->begin(); iter != vector->end(); iter++) {
		glDrawElements(GL_TRIANGLES, iter->length, GL_UNSIGNED_INT, (GLvoid*)(iter->index*sizeof(unsigned int)));
        _stats.draw_calls++;
        _stats.faces += iter->length / 3;
	}
    vector->clear();

	if(Common::GameTime::tickEvery(500, _stats_timer, false)) {
        Common::LiveLog::Builder builder(LOG_STATS_DRAW_CALLS);
        builder.addRefObj("stats", &__LogStatsDrawCallsRefl, (void*)&_stats);
        builder.push();
        _stats.faces = 0;
        _stats.draw_calls = 0;
	}
}