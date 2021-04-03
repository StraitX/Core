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



struct MemoryTypes{
    enum Type: u8{
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

    sx_inline static bool IsMappable(Type type);

    sx_inline static Type ToSupported(GPUMemoryType type, MemoryLayout layout);

};

sx_inline bool MemoryTypes::IsMappable(MemoryTypes::Type type){
    switch (type) {
    case VRAM:
        return false;
    case DynamicVRAM:
    case RAM:
    case UncachedRAM:
        return true;
    }
    return false;
}

sx_inline MemoryTypes::Type MemoryTypes::ToSupported(GPUMemoryType type, MemoryLayout layout){
    CoreAssert(layout != MemoryLayout::Unknown, "Vk: Memory: can't convert to supported type, memory layout is unknown");

    if(layout==MemoryLayout::DedicatedWithDynamic){
        return (MemoryTypes::Type)type;
    }else if(layout == MemoryLayout::Dedicated){
        return MemoryTypes::VRAM;
    }else{// MemoryLayout::Uniform
        return MemoryTypes::RAM;
    }
}

}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_MEMORY_HPP