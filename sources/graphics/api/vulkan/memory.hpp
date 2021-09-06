#ifndef STRAITX_VULKAN_MEMORY_HPP
#define STRAITX_VULKAN_MEMORY_HPP

#include "core/os/vulkan.hpp"
#include "core/types.hpp"
#include "core/defs.hpp"
#include "core/assert.hpp"

namespace Vk{

enum class MemoryLayout{
    Unknown = 0,
    Dedicated,
    DedicatedWithDynamic,
    Uniform
};

struct MemoryType{
    enum Type{
        VRAM = 0,
        DynamicVRAM = 1,
        RAM = 2,
        UncachedRAM = 3
    };

    u32 Index = InvalidIndex;
};

struct MemoryProperties{
    MemoryLayout Layout;
    MemoryType MemoryTypes[4];
    MemoryType::Type RealMemoryTypes[4];

    bool IsMappable(MemoryType::Type type)const;

    static MemoryProperties Get(VkPhysicalDevice device);
};

}//namespace Vk::

#endif //STRAITX_VULKAN_MEMORY_HPP