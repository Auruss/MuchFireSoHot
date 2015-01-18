#pragma once

#include <vector>

namespace OpenGL {
	
	class RenderSystem {

	private:
		struct buffer_settings {
			unsigned int id;
			unsigned int location;
			unsigned int type;
			int count;
		};

		struct render_job {
			unsigned int offset;
			unsigned int count;
		};

	public:
		RenderSystem(unsigned int index_buffer, unsigned int program);

		void addBuffer(unsigned int gl, char* shader_param_name, unsigned int type, int count);

		void enqueueRenderJob(unsigned int offset, unsigned int count);

		void defragedRender();
		void fragedRender();


	private:
		void bindBuffers();

		std::vector<buffer_settings> _buffers;
		unsigned int _program;
		unsigned int _index_buffer;

		std::vector<render_job> _render_jobs;
	};

}