#ifndef STRAITX_VULKAN_SEMAPHORE_HPP
#define STRAITX_VULKAN_SEMAPHORE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "core/assert.hpp"
#include "core/pair.hpp"

namespace StraitX{
namespace Vk{

struct Semaphore{
    VkSemaphore Handle = VK_NULL_HANDLE;
    VkDevice    Owner  = VK_NULL_HANDLE;

    void Create(VkDevice owner);

    void Destroy();
};


sx_inline void Semaphore::Create(VkDevice owner){
    Owner = owner;
    VkSemaphoreCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    CoreAssert(vkCreateSemaphore(Owner, &info, nullptr, &Handle) == VK_SUCCESS, "Vulkan: Can't create semaphore");
}

sx_inline void Semaphore::Destroy(){
    vkDestroySemaphore(Owner, Handle, nullptr);
}



};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SEMAPHORE_HPP