#ifndef STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP
#define STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP

#include "platform/vulkan.hpp"
#include "core/array_ptr.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/vulkan/queue.hpp"
#include "graphics/vulkan/memory.hpp"
#include "graphics/vulkan/gpu_memory_allocator.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/fence.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl: public LogicalGPU{
    VkPhysicalDevice PhysicalHandle = VK_NULL_HANDLE;
    VkDevice  Handle = VK_NULL_HANDLE;

    GPUVendor Vendor = GPUVendor::Unknown;
    GPUType   Type   = GPUType::Unknown;

    u32 GeneralQueueFamily  = InvalidIndex;
    u32 TransferQueueFamily  = InvalidIndex; 

    MemoryTypes Memory;

    GPUMemoryAllocator Allocator;

    Vk::Queue GeneralQueue;
    Vk::Queue TransferQueue;

    Vk::CommandBuffer TransferCmdBuffer;
    Vk::Fence TransferFence;

    static LogicalGPUImpl Instance;

    Result Initialize(const PhysicalGPU &gp)override;

    void Finalize()override;

    void QueryQueues();

    sx_inline VkDeviceMemory Alloc(u32 size, MemoryTypes::Type type){
        return Allocator.Alloc(size,type);
    }

    sx_inline void Free(VkDeviceMemory mem){
        Allocator.Free(mem);
    }
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP