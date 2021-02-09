#include "core/log.hpp"
#include "graphics/vulkan/gpu_memory_allocator.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{
namespace Vk{


void GPUMemoryAllocator::Initialize(LogicalGPUImpl *owner){
    CoreAssert(owner->Handle != VK_NULL_HANDLE, "GPUMemoryAllocator: Passed uninitialized LogicalGPU");
    Owner = owner->Handle;
    Memory = &owner->Memory;
}

void GPUMemoryAllocator::Finalize(){
    (void)0;
}

VkDeviceMemory GPUMemoryAllocator::Alloc(u32 size, MemoryTypes::Type type){
    CoreAssert(Memory->Index[type] != InvalidIndex, "GPUMemoryAllocator: Invalid Memory Type");

    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = size;
    info.memoryTypeIndex = Memory->Index[type];

    VkDeviceMemory memory;
    if(vkAllocateMemory(Owner, &info, nullptr, &memory) != VK_SUCCESS){
        LogWarn("GPUMemoryAllocator: Can't allocate memory");
        return VK_NULL_HANDLE;
    }
    return memory;
}

void GPUMemoryAllocator::Free(VkDeviceMemory memory){
    vkFreeMemory(Owner, memory, nullptr);
}


}//namespace Vk::
}//namespace StraitX::