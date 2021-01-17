#ifndef STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP
#define STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP

#include "graphics/api/gpu_buffer.hpp"

namespace StraitX{
namespace GL{

struct GPUBufferImpl{
    static Result Create(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void Destroy(GPUBuffer &buffer);
};

}//namespace GL::
}//namespace StraitX::

#endif //STRAITX_OPENGL_GPU_BUFFER_IMPL_HPP 