#ifndef STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP
#define STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP

#include "graphics/api/gpu_buffer.hpp"

namespace GL{

struct GPUBufferImpl{
    static void NewImpl(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void DeleteImpl(GPUBuffer &buffer);
};

}//namespace GL::

#endif //STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP 