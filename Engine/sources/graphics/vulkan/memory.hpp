#ifndef STRAITX_VULKAN_MEMORY_HPP
#define STRAITX_VULKAN_MEMORY_HPP


#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"

namespace StraitX{
namespace Vk{

struct LogicalDevice;

struct GPUMemory{
    enum Type: u8{
        Unknown = 0,
        VRAM,
        DynamicVRAM
    };

    LogicalDevice *Owner = nullptr;
    VkDeviceMemory Handle = VK_NULL_HANDLE;
    u32 Size = 0;

    Result Allocate(LogicalDevice *owner, Type type, u32 size);

    void Free();
};


struct CPUMemory{
    enum Type{
        Unknown = 0,
        RAM,
        UncachedRAM
    };
    LogicalDevice *Owner = nullptr;
    VkDeviceMemory Handle = VK_NULL_HANDLE;
    u32 Size = 0;
#ifdef SX_DEBUG
    bool IsMapped = false;
#endif

    Result Allocate(LogicalDevice *owner, Type type, u32 size);

    void Free();

    void *Map(size_t offset);

    void Unmap();
};




};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_MEMORY_HPP