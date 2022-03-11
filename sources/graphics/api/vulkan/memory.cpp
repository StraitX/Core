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
    return  (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        && !(flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
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
        LogInfo("Type[%]\nDevice: %\nVisible: %\nCoherent: %\nCached: %", i,
                bool(props.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                bool(props.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT),
                bool(props.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
                bool(props.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
                );
        if(IsVRAM(props.memoryTypes[i].propertyFlags)){
            result.AbstractMemoryTypes[MemoryType::VRAM].Index = i;
            result.AbstractMemoryTypes[MemoryType::VRAM].BackingMemoryType = MemoryType::VRAM;
            continue;
        }
        if(IsDynamicVRAM(props.memoryTypes[i].propertyFlags)){
            result.AbstractMemoryTypes[MemoryType::DynamicVRAM].Index = i;
            result.AbstractMemoryTypes[MemoryType::DynamicVRAM].BackingMemoryType = MemoryType::DynamicVRAM;
            continue;
        }
        if(IsRAM(props.memoryTypes[i].propertyFlags)){ 
            result.AbstractMemoryTypes[MemoryType::RAM].Index = i;
            result.AbstractMemoryTypes[MemoryType::RAM].BackingMemoryType = MemoryType::RAM;
            continue;
        }
        if(IsUncachedRAM(props.memoryTypes[i].propertyFlags)){
            result.AbstractMemoryTypes[MemoryType::UncachedRAM].Index = i;
            result.AbstractMemoryTypes[MemoryType::UncachedRAM].BackingMemoryType = MemoryType::UncachedRAM;
            continue;
        }
    }

    if(props.memoryTypeCount == 0){
        result.Layout = MemoryLayout::Unknown;
        return result;
    }
    if(props.memoryTypeCount == 1 && IsUniform(props.memoryTypes[0].propertyFlags)){
        result.AbstractMemoryTypes[MemoryType::VRAM].Index = 0;
        result.AbstractMemoryTypes[MemoryType::DynamicVRAM].Index = 0;
        result.AbstractMemoryTypes[MemoryType::RAM].Index = 0;
        result.AbstractMemoryTypes[MemoryType::UncachedRAM].Index = 0;

        // uniform memory has the same capabilities as DynamicRAM
        result.AbstractMemoryTypes[MemoryType::VRAM].BackingMemoryType = MemoryType::DynamicVRAM; 
        result.AbstractMemoryTypes[MemoryType::DynamicVRAM].BackingMemoryType = MemoryType::DynamicVRAM; 
        result.AbstractMemoryTypes[MemoryType::RAM].BackingMemoryType = MemoryType::DynamicVRAM; 
        result.AbstractMemoryTypes[MemoryType::UncachedRAM].BackingMemoryType = MemoryType::DynamicVRAM; 

        result.Layout = MemoryLayout::Uniform;
        return result;
    }
    // check for uniform memory;
    if(result.AbstractMemoryTypes[MemoryType::RAM].Index == InvalidIndex){
        for(int i = 0; i<props.memoryTypeCount; i++){
            if(IsUniform(props.memoryTypes[i].propertyFlags)){
                result.AbstractMemoryTypes[MemoryType::RAM].Index = i;
                result.AbstractMemoryTypes[MemoryType::RAM].BackingMemoryType = MemoryType::DynamicVRAM;
            }
        }
    }
    // first if statement assures that we have RAM and VRAM to fallback to
    if(result.AbstractMemoryTypes[MemoryType::UncachedRAM].Index == InvalidIndex){
        result.AbstractMemoryTypes[MemoryType::UncachedRAM] = result.AbstractMemoryTypes[MemoryType::RAM];
    }
    if(result.AbstractMemoryTypes[MemoryType::DynamicVRAM].Index == InvalidIndex){
        result.AbstractMemoryTypes[MemoryType::DynamicVRAM] = result.AbstractMemoryTypes[MemoryType::VRAM];
        result.Layout = MemoryLayout::Dedicated; 
    }else{
        result.Layout = MemoryLayout::DedicatedWithDynamic;
    }
    return result;
}

bool MemoryProperties::IsMappable(MemoryType::Type abstract_memory_type)const{
    if(Layout == MemoryLayout::Unknown)
        return false;

    if(Layout == MemoryLayout::Uniform)
        return true;

    if(Layout == MemoryLayout::DedicatedWithDynamic){
        if(abstract_memory_type == MemoryType::VRAM)
            return false;
        else
            return true;
    }

    if(Layout == MemoryLayout::Dedicated){
        if(abstract_memory_type == MemoryType::VRAM || abstract_memory_type == MemoryType::DynamicVRAM)
            return false;
        else
            return true;
    }

    return false;
}

}//namespace Vk::