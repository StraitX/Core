#ifndef STRAITX_VULKAN_MEMORY_HPP
#define STRAITX_VULKAN_MEMORY_HPP

#include "platform/vulkan.hpp"
#include "platform/types.hpp"
#include "platform/defs.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{
namespace Vk{

enum class MemoryLayout{
    Unknown = 0,
    Dedicated,
    DedicatedWithDynamic,
    Uniform
};



struct MemoryTypes{
    enum Type{
        VRAM        = 0,
        DynamicVRAM = 1,
        RAM         = 2,
        UncachedRAM = 3
    };

    MemoryLayout Layout = MemoryLayout::Unknown;
    u32 Index[4] = {};
    u64 Size[4]  = {};

    MemoryTypes();

    void Query(VkPhysicalDevice owner);
};



}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_MEMORY_HPP