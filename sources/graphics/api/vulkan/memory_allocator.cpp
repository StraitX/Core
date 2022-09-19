#include "core/log.hpp"
#include "graphics/api/vulkan/memory_allocator.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"

namespace Vk{

VkDeviceMemory MemoryAllocator::Alloc(u32 size, MemoryType::Type type){
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = size;
    info.memoryTypeIndex = GPUImpl::Get().MemoryIndex(type);

    VkDeviceMemory memory;
    if(vkAllocateMemory(GPUImpl::Get(), &info, nullptr, &memory) != VK_SUCCESS){
        LogError("VulkanMemoryAllocator: Can't allocate memory of size '%' of type '%'", size, MemoryType::Name(type));
        return VK_NULL_HANDLE;
    }
    return memory;
}

void MemoryAllocator::Free(VkDeviceMemory memory){
    vkFreeMemory(GPUImpl::Get(), memory, nullptr);
}

}//namespace Vk::