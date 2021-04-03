#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/memory.hpp"

namespace StraitX{
namespace Vk{

sx_inline static bool IsRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}
sx_inline static bool IsUncachedRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

sx_inline static bool IsVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

sx_inline static bool IsDynamicVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

MemoryTypes::MemoryTypes(){
    Index[VRAM]        = 0;
    Index[DynamicVRAM] = 0;
    Index[RAM]         = 0;
    Index[UncachedRAM] = 0;
}

void MemoryTypes::Query(VkPhysicalDevice device){
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(device, &mem_props);

    if(mem_props.memoryTypeCount == 1){
        DLogInfo("Vk: MemoryTypes: Uniform Memory device");
        auto &flags = mem_props.memoryTypes[0].propertyFlags;
        if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
           flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        {
            Layout = MemoryLayout::Uniform;
            Index[RAM] = 0;
            Size[RAM] = mem_props.memoryHeaps[mem_props.memoryTypes[0].heapIndex].size;
            return;
        }
        DLogInfo("Vk: MemoryTypes: Uniform Memory device: Unsupported Memory type");
        return;
    }
    
    int i = 0; 
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsRAM(mem_props.memoryTypes[i])){
            Size[RAM] = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            Index[RAM] = i++;
            DLogInfo("Vk: MemoryTypes: RAM memory type was found:            Size: % MB",Size[RAM]/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsUncachedRAM(mem_props.memoryTypes[i])){
            Size[UncachedRAM] = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            Index[UncachedRAM] = i++;
            DLogInfo("Vk: MemoryTypes: UncachedRAM memory type was found:    Size: % MB",Size[UncachedRAM]/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsVRAM(mem_props.memoryTypes[i])){
            Size[VRAM]= mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            Index[VRAM] = i++;
            DLogInfo("Vk: MemoryTypes: VRAM memory type was found:           Size: % MB",Size[VRAM]/float(1024*1024)); 
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsDynamicVRAM(mem_props.memoryTypes[i])){
            Size[DynamicVRAM] = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            Index[DynamicVRAM] = i++;
            DLogInfo("Vk: MemoryTypes: DynamicVRAM memory type was found:    Size: % MB",Size[DynamicVRAM]/float(1024*1024));
            break;
        }
    }   

    if(Index[VRAM] == InvalidIndex)return;
    if(Index[RAM]  == InvalidIndex)return;

    Layout = (Index[DynamicVRAM] != InvalidIndex) ? Layout = MemoryLayout::DedicatedWithDynamic : MemoryLayout::Dedicated;
}

}//namespace Vk::
}//namespace StraitX::