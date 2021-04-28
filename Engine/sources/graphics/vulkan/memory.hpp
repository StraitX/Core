#ifndef STRAITX_VULKAN_MEMORY_HPP
#define STRAITX_VULKAN_MEMORY_HPP

#include "platform/vulkan.hpp"
#include "platform/types.hpp"
#include "platform/defs.hpp"
#include "core/assert.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{
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

MemoryType::Type ToVkMemoryType(GPUMemoryType type);

struct MemoryProperties{
    MemoryLayout Layout;
    MemoryType Memory[4];

    static MemoryProperties Get(VkPhysicalDevice device);
};

}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_MEMORY_HPP