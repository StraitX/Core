#ifndef STRAITX_VULKAN_QUEUE_HPP
#define STRAITX_VULKAN_QUEUE_HPP

#include "core/os/vulkan.hpp"
#include "core/result.hpp"
#include "core/types.hpp"
#include "core/defs.hpp"

namespace Vk{

struct QueueFamily{
    enum Type{
        Graphics = 0,
        Compute  = 1,
        Transfer = 2,
    };

    static constexpr size_t FamilyCount = 3;

    u32 Index;
    u32 Count;

    QueueFamily(u32 index = InvalidIndex, u32 count = 0):
        Index(index),
        Count(count)
    {}
};

struct QueueProperties{
    QueueFamily Family[QueueFamily::FamilyCount];

    static QueueProperties Get(VkPhysicalDevice device);
};

struct AbstractQueue{
    QueueFamily *BackingQueueFamily = nullptr;
    VkQueue Handle = VK_NULL_HANDLE;
};

}//namespace Vk::

#endif //STRAITX_VULKAN_QUEUE_HPP