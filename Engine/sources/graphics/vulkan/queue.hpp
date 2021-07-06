#ifndef STRAITX_VULKAN_QUEUE_HPP
#define STRAITX_VULKAN_QUEUE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"
#include "platform/defs.hpp"

namespace Vk{

struct QueueFamily{
    enum Type{
        Graphics = 0,
        Compute  = 1,
        Transfer = 2,
        FamilyCount = 3
    };

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

}//namespace Vk::

#endif //STRAITX_VULKAN_QUEUE_HPP