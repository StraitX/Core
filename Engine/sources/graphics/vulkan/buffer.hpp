#ifndef STRAITX_VULKAN_BUFFER_HPP
#define STRAITX_VULKAN_BUFFER_HPP


#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"

namespace StraitX{
namespace Vk{

struct LogicalDevice;
struct CPUMemory;
struct GPUMemory;

struct Buffer{
    Vk::LogicalDevice *Owner = nullptr;
    VkBuffer Handle = VK_NULL_HANDLE;
    u32 Size = 0;

    Result Create(Vk::LogicalDevice *owner, u32 size, VkBufferUsageFlags usage);

    void Bind(const GPUMemory &memory, u32 offset);

    void Bind(const CPUMemory &memory, u32 offset);

    u64 MemoryRequirements();

    void Destroy();
};


};//namespace Vk::
};//namespace StraitX::



#endif // STRAITX_VULKAN_BUFFER_HPP