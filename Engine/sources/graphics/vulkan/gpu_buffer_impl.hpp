#ifndef STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GPUBufferImpl{
    LogicalGPU *&Owner;
    VkBuffer &Handle;
    VkDeviceMemory &Memory;
    u32 &Size;
    GPUBuffer::UsageType &Usage;

    constexpr GPUBufferImpl(GPUBuffer &buffer);

    constexpr GPUBufferImpl(LogicalGPU *&owner, VkBuffer &Handle, VkDeviceMemory &Memory, u32 &size, GPUBuffer::UsageType &usage);

    void Create(LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    void Destroy();

    static void NewImpl(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void DeleteImpl(GPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP