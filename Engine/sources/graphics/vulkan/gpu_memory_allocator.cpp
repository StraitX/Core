#include "core/log.hpp"
#include "graphics/vulkan/gpu_memory_allocator.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{
namespace Vk{

sx_inline bool IsRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}
sx_inline bool IsUncachedRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

sx_inline bool IsVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

sx_inline bool IsDynamicVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

void GPUMemoryAllocator::Initialize(LogicalGPUImpl *owner){
    Owner = owner;

    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(Owner->PhysicalHandle, &mem_props);

    if(mem_props.memoryTypeCount == 1){
        DLogInfo("VulkanAllocator: Exceptional Device: Fallback to one memory type");
        auto &flags = mem_props.memoryTypes[0].propertyFlags;
        if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
           flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        {
            RAM = UncachedRAM = VRAM = DynamicVRAM = 0;
            RAMSize = UncachedRAMSize = VRAMSize = DynamicVRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[0].heapIndex].size;
            return;
        }
        DLogWarn("VulkanAllocator: Exceptional Device: Unsupported Memory");
        return;
    }
    
    int i = 0; 
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsRAM(mem_props.memoryTypes[i])){
            RAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            RAM = i++;
            DLogInfo("VulkanAllocator: RAM memory type was found:            Size: % MB",RAMSize/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsUncachedRAM(mem_props.memoryTypes[i])){
            UncachedRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            UncachedRAM = i++;
            DLogInfo("VulkanAllocator: UncachedRAM memory type was found:    Size: % MB",UncachedRAMSize/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsVRAM(mem_props.memoryTypes[i])){
            VRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            VRAM = i++;
            DLogInfo("VulkanAllocator: VRAM memory type was found:           Size: % MB",VRAMSize/float(1024*1024)); 
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsDynamicVRAM(mem_props.memoryTypes[i])){
            DynamicVRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            DynamicVRAM = i++;
            DLogInfo("VulkanAllocator: DynamicVRAM memory type was found:    Size: % MB",DynamicVRAMSize/float(1024*1024));
            break;
        }
    }   
}

void GPUMemoryAllocator::Finalize(){
    Owner = nullptr;
}

u32 GPUMemoryAllocator::GetMemoryIndex(GPUMemoryType type){
    if(type == GPUMemoryType::DynamicVRAM)
        if(DynamicVRAM)
            return DynamicVRAM;
    return VRAM;
}

VkDeviceMemory GPUMemoryAllocator::Alloc(u32 size, GPUMemoryType type){
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = size;
    info.memoryTypeIndex = GetMemoryIndex(type);

    VkDeviceMemory memory;
    if(vkAllocateMemory(Owner->Handle, &info, nullptr, &memory) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    return memory;
}

void GPUMemoryAllocator::Free(VkDeviceMemory memory){
    vkFreeMemory(Owner->Handle, memory, nullptr);
}


}//namespace Vk::
}//namespace StraitX::