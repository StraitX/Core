#ifndef STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP
#define STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP

#include "platform/vulkan.hpp"
#include "platform/defs.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/vulkan/memory.hpp"

namespace StraitX{
namespace Vk{

struct MemoryAllocator{
    static VkDeviceMemory Alloc(u32 size, MemoryType::Type type);

    static void Free(VkDeviceMemory memory);
};

}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP 