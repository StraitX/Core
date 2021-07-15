#ifndef STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/gpu_buffer.hpp"

namespace Vk{

struct GPUBufferImpl{
    VkBuffer &Handle;
    VkDeviceMemory &Memory;
    u32 &Size;
    GPUBuffer::UsageType &Usage;
    GPUMemoryType &MemoryType;

    GPUBufferImpl(GPUBuffer &buffer);

    GPUBufferImpl(VkBuffer &Handle, VkDeviceMemory &Memory, u32 &size, GPUBuffer::UsageType &usage, GPUMemoryType &mem_type);

    void Create(u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    void Destroy();

    static void NewImpl(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void DeleteImpl(GPUBuffer &buffer);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP