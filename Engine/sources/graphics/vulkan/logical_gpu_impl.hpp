#ifndef STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP
#define STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/vulkan/queue.hpp"
#include "graphics/vulkan/gpu_memory_allocator.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/semaphore.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl: public LogicalGPU{
    VkPhysicalDevice PhysicalHandle = VK_NULL_HANDLE;
    VkDevice  Handle = VK_NULL_HANDLE;

    GPUVendor Vendor = GPUVendor::Unknown;
    GPUType   Type   = GPUType::Unknown;

    u32 GraphicsQueueFamily  = InvalidIndex;
    u32 ComputeQueueFamily   = InvalidIndex;
    u32 TransferQueueFamily  = InvalidIndex; 

    GPUMemoryAllocator Allocator;

    Vk::Queue GraphicsQueue;
    Vk::Queue TransferQueue;

    Vk::CommandPool   CmdPool;
    Vk::CommandBuffer CmdBuffer;

    virtual Result Create(const PhysicalGPU &gpu)override;

    virtual void Destroy()override;

    void QueryQueues();

    sx_inline VkDeviceMemory Alloc(u32 size, GPUMemoryType type){
        return Allocator.Alloc(size,type);
    }

    sx_inline void Free(VkDeviceMemory mem){
        Allocator.Free(mem);
    }
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP