#ifndef STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP
#define STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP

#include "platform/vulkan.hpp"
#include "platform/defs.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl;

struct GPUMemoryAllocator{
    LogicalGPUImpl *Owner = nullptr;

    u32 VRAM            = InvalidIndex;
    u64 VRAMSize        = 0;

    u32 DynamicVRAM     = InvalidIndex;
    u64 DynamicVRAMSize = 0;

    u32 RAM             = InvalidIndex;
    u64 RAMSize         = 0;

    u32 UncachedRAM     = InvalidIndex;
    u64 UncachedRAMSize = 0;

    void Initialize(LogicalGPUImpl *owner);

    void Finalize();

    sx_inline u32 GetMemoryIndex(GPUMemoryType type);

    VkDeviceMemory Alloc(u32 size, GPUMemoryType type);

    void Free(VkDeviceMemory memory);
};

}//namespace Vk::
}//namespace StraitX::

#endif //STRAITX_VULKAN_GPU_MEMORY_ALLOCATOR_HPP 