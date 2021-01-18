#ifndef STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/gpu_buffer.hpp"

namespace StraitX{
namespace Vk{

struct GPUBufferImpl{
    sx_inline static u32 GetBufferMemoryRequirements(const GPUBuffer &buffer);

    sx_inline static Result AcquireMemory(GPUBuffer &buffer, GPUMemoryType type);

    sx_inline static void ReleaseMemory(GPUBuffer &buffer);

    static Result Create(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void Destroy(GPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP