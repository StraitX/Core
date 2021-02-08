#ifndef STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GPUBufferImpl{
    Vk::LogicalGPUImpl *const Owner;
    VkBuffer &Handle;
    VkDeviceMemory &Memory;
    u32 &Size;

    constexpr GPUBufferImpl(GPUBuffer &buffer);

    constexpr GPUBufferImpl(Vk::LogicalGPUImpl *const owner, VkBuffer &Handle, VkDeviceMemory &Memory, u32 &size);

    sx_inline void Create(u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    sx_inline void Destroy();

    sx_inline static u32 GetBufferMemoryRequirements(VkDevice owner, VkBuffer buffer);

    static void NewImpl(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage);

    static void DeleteImpl(GPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_BUFFER_IMPL_HPP