#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <Common/GameTime.h>
#include <Common/LiveLog/Builder.h>
#include <Storage/IGpuBuffer.h>
#include <Storage/GpuBuffer.h>
#include <Storage/Defragmenter.h>

namespace OpenGL {

	class RenderSystem {

	private:
		struct buffer_settings {
            Storage::IGpuBuffer* owner;
			unsigned int location;
			unsigned int type;
			int count;
		};

		struct render_job {
			unsigned int offset;
			unsigned int count;
		};

        struct LogStatsDrawCalls {
            int draw_calls;
            int faces;
        };

    public:
        struct Combined {
            int index;
            int vindex;
        };

	public:
		RenderSystem(unsigned int program, Storage::GpuBuffer<unsigned int>* index_buffer);

        template <class T>
        void addGpuBuffer(const char* program_eq, Storage::GpuBuffer<T>* buffer);

        Combined requestCombined(int vertices, int indices);

		void enqueueRenderJob(unsigned int offset, unsigned int count);

		void render();

        unsigned int getProgram() { return _program; }

	private:
        unsigned int attribLocation(const char* eq);
		void bindBuffers();

		std::vector<buffer_settings> _buffers;
		unsigned int _program;
        Storage::GpuBuffer<unsigned int>* _index_buffer;
        Storage::Defragmenter _render_jobs;

        LogStatsDrawCalls _stats;
        GameTimeObj _stats_timer;
	};


    template <class T>
    void RenderSystem::addGpuBuffer(const char* program_eq, Storage::GpuBuffer<T>* buffer) {
        buffer_settings set;
        set.owner = (Storage::IGpuBuffer*) buffer;
        set.location = attribLocation(program_eq);

        if(typeid(T) == typeid(glm::vec4)) {
            set.type = GL_FLOAT;
            set.count = 4;
        } else if (typeid(T) == typeid(glm::vec3)) {
            set.type = GL_FLOAT;
            set.count = 3;
        } else if (typeid(T) == typeid(glm::ivec3)) {
            set.type = GL_INT;
            set.count = 3;
        } else if(typeid(T) == typeid(glm::vec2)) {
            set.type = GL_FLOAT;
            set.count = 2;
        } else {
            Common::LiveLog::Builder builder(LOG_CRITICAL_RENDER_ERROR);
            builder.setMessage("[RenderSystem] Unknown gpu buffer type: '%s'", typeid(T).name());
            builder.push();
            return;
        }

        _buffers.push_back(set);
    }
}