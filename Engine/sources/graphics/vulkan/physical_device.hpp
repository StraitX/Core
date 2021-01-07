#ifndef STRAITX_VULKAN_PHYSICAL_DEVICE_HPP
#define STRAITX_VULKAN_PHYSICAL_DEVICE_HPP

#include "platform/vulkan.hpp"
#include "platform/types.hpp"
#include "platform/defs.hpp"
#include "platform/result.hpp"

namespace StraitX{
namespace Vk{

enum class DeviceType: u8{
    Unknown,
    DiscreteGPU,
    IntegratedGPU
};

struct PhysicalDevice{
    VkPhysicalDevice Handle = VK_NULL_HANDLE;

    DeviceType Type = DeviceType::Unknown;

    u32 VRAM        = InvalidIndex;
    size_t VRAMSize =  0;
    //optional
    u32 DynamicVRAM = InvalidIndex;
    size_t DynamicVRAMSize = 0;

    u32 RAM         = InvalidIndex;
    size_t RAMSize  =  0;
    //optional
    u32 UncachedRAM = InvalidIndex;
    size_t UncachedRAMSize = 0;


    // TODO somehow count available queues amount after fallback
    u32 GraphicsQueueFamily  = InvalidIndex;
    u32 ComputeQueueFamily   = InvalidIndex;
    u32 TransferQueueFamily  = InvalidIndex; 

    void Create(VkPhysicalDevice device);

    void QueryMemoryTypes();

    void QueryQueues();

    //minimal supported gpu should have all three queues available
    // and at least VRAM and RAM memory types
    sx_inline bool IsComplete()const{
        return GraphicsQueueFamily != InvalidIndex
        &&      ComputeQueueFamily != InvalidIndex
        &&     TransferQueueFamily != InvalidIndex
        
        &&      RAM != InvalidIndex
        &&     VRAM != InvalidIndex;
    }

};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_PHYSICAL_DEVICE_HPP