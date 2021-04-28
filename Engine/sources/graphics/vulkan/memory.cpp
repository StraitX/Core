#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/memory.hpp"

namespace StraitX{
namespace Vk{

MemoryType::Type ToVkMemoryType(GPUMemoryType type){
    return (MemoryType::Type)type;
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
            result.Memory[MemoryType::VRAM].Index = i;
            continue;
        }
        if(IsDynamicVRAM(props.memoryTypes[i].propertyFlags)){
            result.Memory[MemoryType::DynamicVRAM].Index = i;
            continue;
        }
        if(IsRAM(props.memoryTypes[i].propertyFlags)){ 
            result.Memory[MemoryType::RAM].Index = i;
            continue;
        }
        if(IsUncachedRAM(props.memoryTypes[i].propertyFlags)){
            result.Memory[MemoryType::UncachedRAM].Index = i;
            continue;
        }
    }

    if(props.memoryTypeCount == 0 || result.Memory[MemoryType::RAM].Index == InvalidIndex || result.Memory[MemoryType::VRAM].Index == InvalidIndex){
        result.Layout = MemoryLayout::Unknown;
        return result;
    }
    if(props.memoryTypeCount == 1 && IsUniform(props.memoryTypes[0].propertyFlags)){
        result.Memory[MemoryType::VRAM].Index = 0;
        result.Memory[MemoryType::DynamicVRAM].Index = 0;
        result.Memory[MemoryType::RAM].Index = 0;
        result.Memory[MemoryType::UncachedRAM].Index = 0;

        result.Layout = MemoryLayout::Uniform;
        return result;
    }
    // first if statement assures that we have RAM and VRAM to fallback to
    if(result.Memory[MemoryType::UncachedRAM].Index == InvalidIndex){
        result.Memory[MemoryType::UncachedRAM] = result.Memory[MemoryType::RAM];
    }
    if(result.Memory[MemoryType::DynamicVRAM].Index == InvalidIndex){
        result.Memory[MemoryType::DynamicVRAM] = result.Memory[MemoryType::VRAM];
        result.Layout = MemoryLayout::Dedicated; 
    }else{
        result.Layout = MemoryLayout::DedicatedWithDynamic;
    }
    return result;
}

bool MemoryProperties::IsMappable(MemoryType::Type type)const{
    Assert(Layout != MemoryLayout::Unknown);

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
}//namespace StraitX::