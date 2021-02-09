#ifndef STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP
#define STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP

#include "platform/vulkan.hpp"
#include "platform/defs.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/vulkan/memory.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl;

struct GPUMemoryAllocator{
    VkDevice     Owner  = VK_NULL_HANDLE;
    MemoryTypes *Memory = nullptr;

    void Initialize(LogicalGPUImpl *owner);

    void Finalize();

    VkDeviceMemory Alloc(u32 size, MemoryTypes::Type type);

    void Free(VkDeviceMemory memory);
};

}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP 