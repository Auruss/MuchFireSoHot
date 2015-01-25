#pragma once

#include <Common/GameTime.h>
#include <Storage/Defragmenter.h>
#include <Common/LiveLog/Builder.h>

#include <vector>
#include <cstdio>

#include <GL/glew.h>

namespace Storage {

    struct GpuStatsLog {
        int buffer_id;
        int bytes;
        const char* buffer_type;
    };

	template <class T>
	class GpuBuffer {

	public:
		
		/**
			Creates an memory buffer on the GPU with max*sizeof(T) bytes

			@param max maximum elements of type T
			@param gl_type OpenGL Buffer Type (see opengl docs)
		*/
		GpuBuffer(unsigned int max, unsigned int gl_type);
		~GpuBuffer();

		/**
			Requests a buffer of maximum "max_elements" elements

			@param max_elements Max element count
			@returns A simple identifier for further use
		*/
		int request(int max_elements);

		/**
			Releases a buffer

			@param id identifier that was returned from 'request'
			@param offset buffer offset 
			@param amount of elements to release
		*/
		void release(int id, int offset, int count);

		/**
			Resizes the buffer

			@param id identifier that was returned from 'request'
			@param from original buffer size
			@param to new buffer size
		*/
		void resize(int id, int from, int to);


		unsigned int getInternId() { return _gl_id; }

		// --------------------------------------------------------------------------

		void beginUpdate(int id, int offset = 0);
		void endUpdate();

		T* next();

		// --------------------------------------------------------------------------
		
		/**
			Uploads all changes to the gpu
		*/
		void uploadChanges();

		// --------------------------------------------------------------------------
		
	private:
		struct part {
			int index;
			int length;
		};
		unsigned int _gl_type;
		unsigned int _gl_id;
		T* _data;
		Storage::Defragmenter _update_defragmenter;
		Storage::Defragmenter _free_buffers;
		
		int _start;
		int _current;

        GameTimeObj _stats_time;
        GpuStatsLog _stats;
	};

	// ############################################################################################
    static Common::LiveLog::ReflObject __GpuStatsLogRefl;
    static bool __ReflImplemented = false;

	template <class T>
	GpuBuffer<T>::GpuBuffer(unsigned int max, unsigned int gl_type) {
        if(!__ReflImplemented) {
            __GpuStatsLogRefl.init<GpuStatsLog>();
            __GpuStatsLogRefl.addMember<int>("buffer_id", offsetof(GpuStatsLog,buffer_id));
            __GpuStatsLogRefl.addMember<int>("bytes", offsetof(GpuStatsLog,bytes));
            __GpuStatsLogRefl.addMember<const char*>("buffer_type", offsetof(GpuStatsLog,buffer_type));
            if(__GpuStatsLogRefl.sizeOf() != sizeof(GpuStatsLog)) {
                fprintf(stderr, "Reflection failed for GpuBuffer\n");
            }
            __ReflImplemented = true;
        }

		_gl_type = gl_type;
		_data = new T[max];

		_free_buffers.addFragment(0, max);

		// allocate all memory on gpu
		glGenBuffers(1, &_gl_id);
		glBindBuffer(_gl_type, _gl_id);
		glBufferData(_gl_type, sizeof(T) * max, NULL, GL_DYNAMIC_DRAW);

        _stats.buffer_id = _gl_id;
        _stats.buffer_type = typeid(T).name();
        _stats.bytes = 0;
	}

	template <class T>
	GpuBuffer<T>::~GpuBuffer() {
		delete[] _data;
	}

	// ############################################################################################

	template <class T>
	int GpuBuffer<T>::request(int max_elements) {
		// loop through all free buffers
		unsigned int count;
		auto data = _free_buffers.defrag(&count);
		for(auto iter = data->begin(); iter != data->end(); iter++) {
			// fits perfectly
			if(iter->length == max_elements) {
				int id = iter->index;
				data->erase(iter);
				return id;
			}

			// split is required -> insert in front
			if(iter->length > max_elements) {
				int id = iter->index;
				iter->index += max_elements;
				iter->length -= max_elements;
				return id;
			}
		}

		// no space
		fprintf(stderr, "GpuBuffer failed to find free space for request (count=%d)\n", max_elements);
		return -1;
	}

	template <class T>
	void GpuBuffer<T>::release(int id, int offset, int count) {
		_free_buffers.addFragment(id + offset, count);
	}
	
	template <class T>
	void GpuBuffer<T>::resize(int id, int from, int to) {
		// make it smaller
		if(from > to) {
			this->release(id, to, from - to);
		} 
		else {
			// TODO: NOT IMPLEMENTED YET
			fprintf(stderr, "GpuBuffer::resize has no behavior for making the buffer bigger implemented yet!\n");
		}
	}
	
	// ############################################################################################

	template <class T>
	void GpuBuffer<T>::beginUpdate(int id, int offset) {
		_start = id + offset;
		_current = 0;
	}

	template <class T>
	void GpuBuffer<T>::endUpdate() {
		_update_defragmenter.addFragment(_start, _current);
	}

	template <class T>
	T* GpuBuffer<T>::next() {
		return &_data[_start + _current++];
	}

	// ############################################################################################

	template <class T>
	void GpuBuffer<T>::uploadChanges() {
		glBindBuffer(_gl_type, _gl_id);

		unsigned int count = 0;
		auto updates = _update_defragmenter.defrag(&count);
		for(auto iter = updates->begin(); iter != updates->end(); iter++) {
            _stats.bytes += iter->length * sizeof(T);
			glBufferSubData(_gl_type, sizeof(T) * iter->index, sizeof(T) * iter->length, (void*)(_data+iter->index));
		}
		updates->clear();

		if(Common::GameTime::tickEvery(500, _stats_time, false)) {
            Common::LiveLog::Builder builder(LOG_STATS_GPU_BUFFER);
            builder.addRefObj("stats", &__GpuStatsLogRefl, (void*)&_stats);
            builder.push();
            _stats.bytes = 0;
		}
	}
}