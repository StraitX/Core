#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/api/vulkan/memory.hpp"

namespace Vk{

const char *MemoryType::Name(Type type){
    const char *s_Names[]={
        "VRAM",
        "DynamicVRAM",
        "RAM",
        "UncachedRAM"
    };

    return s_Names[(size_t)type];
}

static bool IsVRAM(VkMemoryPropertyFlags flags){
    return  (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        && !(flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

static bool IsDynamicVRAM(VkMemoryPropertyFlags flags){
    return  (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

static bool IsRAM(VkMemoryPropertyFlags flags){
    return !(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

static bool IsUncachedRAM(VkMemoryPropertyFlags flags){
    return !(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

static bool IsUniform(VkMemoryPropertyFlags flags){
    return  (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
        &&  (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

MemoryProperties MemoryProperties::Get(VkPhysicalDevice device){
    MemoryProperties result;

    VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(device, &props);

    for(u32 i = 0; i<props.memoryTypeCount; i++){
        if(IsVRAM(props.memoryTypes[i].propertyFlags)){
            result.MemoryTypes[MemoryType::VRAM].Index = i;
            result.RealMemoryTypes[MemoryType::VRAM] = MemoryType::VRAM;
            continue;
        }
        if(IsDynamicVRAM(props.memoryTypes[i].propertyFlags)){
            result.MemoryTypes[MemoryType::DynamicVRAM].Index = i;
            result.RealMemoryTypes[MemoryType::DynamicVRAM] = MemoryType::DynamicVRAM;
            continue;
        }
        if(IsRAM(props.memoryTypes[i].propertyFlags)){ 
            result.MemoryTypes[MemoryType::RAM].Index = i;
            result.RealMemoryTypes[MemoryType::RAM] = MemoryType::RAM;
            continue;
        }
        if(IsUncachedRAM(props.memoryTypes[i].propertyFlags)){
            result.MemoryTypes[MemoryType::UncachedRAM].Index = i;
            result.RealMemoryTypes[MemoryType::UncachedRAM] = MemoryType::UncachedRAM;
            continue;
        }
    }

    if(props.memoryTypeCount == 0 || result.MemoryTypes[MemoryType::RAM].Index == InvalidIndex || result.MemoryTypes[MemoryType::VRAM].Index == InvalidIndex){
        result.Layout = MemoryLayout::Unknown;
        return result;
    }
    if(props.memoryTypeCount == 1 && IsUniform(props.memoryTypes[0].propertyFlags)){
        result.MemoryTypes[MemoryType::VRAM].Index = 0;
        result.MemoryTypes[MemoryType::DynamicVRAM].Index = 0;
        result.MemoryTypes[MemoryType::RAM].Index = 0;
        result.MemoryTypes[MemoryType::UncachedRAM].Index = 0;

        // uniform memory has the same capabilities as DynamicRAM
        result.RealMemoryTypes[MemoryType::VRAM] = MemoryType::DynamicVRAM; 
        result.RealMemoryTypes[MemoryType::DynamicVRAM] = MemoryType::DynamicVRAM; 
        result.RealMemoryTypes[MemoryType::RAM] = MemoryType::DynamicVRAM; 
        result.RealMemoryTypes[MemoryType::UncachedRAM] = MemoryType::DynamicVRAM; 

        result.Layout = MemoryLayout::Uniform;
        return result;
    }
    // first if statement assures that we have RAM and VRAM to fallback to
    if(result.MemoryTypes[MemoryType::UncachedRAM].Index == InvalidIndex){
        result.MemoryTypes[MemoryType::UncachedRAM] = result.MemoryTypes[MemoryType::RAM];
        result.RealMemoryTypes[MemoryType::UncachedRAM] = MemoryType::RAM;
    }
    if(result.MemoryTypes[MemoryType::DynamicVRAM].Index == InvalidIndex){
        result.MemoryTypes[MemoryType::DynamicVRAM] = result.MemoryTypes[MemoryType::VRAM];
        result.RealMemoryTypes[MemoryType::DynamicVRAM] = MemoryType::VRAM;
        result.Layout = MemoryLayout::Dedicated; 
    }else{
        result.Layout = MemoryLayout::DedicatedWithDynamic;
    }
    return result;
}

bool MemoryProperties::IsMappable(MemoryType::Type type)const{
    SX_ASSERT(Layout != MemoryLayout::Unknown);

    if(Layout == MemoryLayout::Uniform)
        return true;

    if(Layout == MemoryLayout::DedicatedWithDynamic){
        if(type == MemoryType::VRAM)
            return false;
        else
            return true;
    }

    if(Layout == MemoryLayout::Dedicated){
        if(type == MemoryType::VRAM || type == MemoryType::DynamicVRAM)
            return false;
        else
            return true;
    }

    return false;
}

}//namespace Vk::