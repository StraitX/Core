#include "core/log.hpp"
#include "graphics/vulkan/memory_allocator.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

VkDeviceMemory MemoryAllocator::Alloc(u32 size, MemoryType::Type type){
    VkMemoryAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.allocationSize = size;
    info.memoryTypeIndex = GPU::Get().MemoryIndex(type);

    VkDeviceMemory memory;
    if(vkAllocateMemory(GPU::Get().Handle(), &info, nullptr, &memory) != VK_SUCCESS){
        LogWarn("GPUMemoryAllocator: Can't allocate memory");
        return VK_NULL_HANDLE;
    }
    return memory;
}

void MemoryAllocator::Free(VkDeviceMemory memory){
    vkFreeMemory(GPU::Get().Handle(), memory, nullptr);
}


}//namespace Vk::
}//namespace StraitX::