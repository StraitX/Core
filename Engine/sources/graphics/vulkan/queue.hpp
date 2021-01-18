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

    sx_inline void Obtain(VkDevice owner, u32 family_index, u32 index){
        FamilyIndex = family_index;
        vkGetDeviceQueue(owner, FamilyIndex, index, &Handle);
    }

    sx_inline bool IsInitialized(){
        return Handle != VK_NULL_HANDLE;
    }
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_QUEUE_HPP