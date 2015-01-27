#pragma once

namespace Storage {

    class IGpuBuffer {
    public:
        virtual int request(int elements) = 0;

        virtual void release(int id, int offset, int count) = 0;

        virtual void resize(int id, int old, int new_size) = 0;

        virtual unsigned int getInternId() = 0;
    };

}