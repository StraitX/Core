#ifndef STRAITX_VULKAN_QUEUE_HPP
#define STRAITX_VULKAN_QUEUE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"

namespace StraitX{
namespace Vk{

struct Queue{
    VkQueue Handle = VK_NULL_HANDLE;
    u32 FamilyIndex = -1;

    Queue() = default;

    sx_inline Queue(VkQueue queue, u32 family_index):
        Handle(queue),
        FamilyIndex(family_index)
    {}
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_QUEUE_HPP