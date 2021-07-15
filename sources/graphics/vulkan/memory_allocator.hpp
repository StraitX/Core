#ifndef STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP
#define STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP

#include "platform/vulkan.hpp"
#include "platform/defs.hpp"
#include "graphics/gpu_configuration.hpp"
#include "graphics/vulkan/memory.hpp"

namespace Vk{

struct MemoryAllocator{
    static VkDeviceMemory Alloc(u32 size, MemoryType::Type type);

    static void Free(VkDeviceMemory memory);
};

}//namespace Vk::

#endif //STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP 