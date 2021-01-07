#include "core/log.hpp"
#include "graphics/vulkan/memory.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

Result GPUMemory::Allocate(LogicalDevice *owner, Type type, u32 size){
    Owner = owner;
    Size = size;

    u32 memory_index = InvalidIndex;
    switch (type) {
    case Type::Unknown: 
        return Result::Failure;
    case Type::VRAM: 
        memory_index = owner->Parent->VRAM; 
        break;
    case Type::DynamicVRAM:
        if(owner->Parent->DynamicVRAM != InvalidIndex){
            memory_index = owner->Parent->DynamicVRAM;
        }else{
            LogWarn("Vulkan: GPUMemory: DynamicVRAM: Fallback to VRAM");
            memory_index = owner->Parent->VRAM;
        }
        break;
    }
    CoreAssert(memory_index != InvalidIndex, "Vulkan: GPUMemory: can't find appropriate memory type");

    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = Size;
    info.memoryTypeIndex = memory_index;

    return ResultError(vkAllocateMemory(Owner->Handle, &info, nullptr, &Handle));
}

void GPUMemory::Free(){
    vkFreeMemory(Owner->Handle, Handle, nullptr);
#ifdef SX_DEBUG
    Handle = VK_NULL_HANDLE;
    Owner = nullptr;
    Size = 0;
#endif
}


Result CPUMemory::Allocate(LogicalDevice *owner, Type type, u32 size){
    Owner = owner;
    Size = size;

    u32 memory_index = InvalidIndex;

    switch (type) {
    case Type::Unknown: 
        return Result::Failure;
    case Type::RAM: 
        memory_index = Owner->Parent->RAM; 
        break;
    case Type::UncachedRAM:
        if(Owner->Parent->UncachedRAM != InvalidIndex){
            memory_index = Owner->Parent->UncachedRAM;
        }else{
            memory_index = Owner->Parent->RAM;
            LogWarn("Vulkan: CPUMemory: UncachedRAM: Fallback to RAM");
        }
        break;
    }

    CoreAssert(memory_index != InvalidIndex, "Vulkan: CPUMemory: can't find appropriate memory type");

    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = Size;
    info.memoryTypeIndex = memory_index;

    return ResultError(vkAllocateMemory(Owner->Handle, &info, nullptr, &Handle));
}

void CPUMemory::Free(){
    vkFreeMemory(Owner->Handle, Handle, nullptr);

#ifdef SX_DEBUG 
    CoreAssert(IsMapped == false, "Vulkan: CPUMemory: attempt to free mapped memory chunk");
    Handle = VK_NULL_HANDLE;
    Owner = nullptr;
    Size = 0;
#endif
}

void *CPUMemory::Map(size_t offset){
#ifdef SX_DEBUG
    IsMapped = true;
#endif
    void *ptr = nullptr;
    vkMapMemory(Owner->Handle, Handle, offset, Size, 0, &ptr);
    return ptr;
}

void CPUMemory::Unmap(){
#ifdef SX_DEBUG
    IsMapped = false;
#endif
    vkUnmapMemory(Owner->Handle, Handle);
}


};//namespace Vk::
};//namespace StraitX::