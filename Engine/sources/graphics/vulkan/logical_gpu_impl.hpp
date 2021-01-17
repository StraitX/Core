#ifndef STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP
#define STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl: public LogicalGPU{
    VkPhysicalDevice PhysicalHandle = VK_NULL_HANDLE;
    VkDevice Handle = VK_NULL_HANDLE;

    GPUVendor Vendor = GPUVendor::Unknown;
    GPUType Type = GPUType::Unknown;

    u32 VRAM            = InvalidIndex;
    u64 VRAMSize        = 0;

    u32 DynamicVRAM     = InvalidIndex;
    u64 DynamicVRAMSize = 0;

    u32 RAM             = InvalidIndex;
    u64 RAMSize         = 0;

    u32 UncachedRAM     = InvalidIndex;
    u64 UncachedRAMSize = 0;

    u32 GraphicsQueueFamily  = InvalidIndex;
    u32 ComputeQueueFamily   = InvalidIndex;
    u32 TransferQueueFamily  = InvalidIndex; 


    Vk::Queue GraphicsQueue;

    virtual Result Create(const PhysicalGPU &gpu)override;

    virtual void Destroy()override;

    void QueryMemoryTypes();

    void QueryQueues();
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP