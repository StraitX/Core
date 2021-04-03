#ifndef STRAITX_VULKAN_FENCE_HPP
#define STRAITX_VULKAN_FENCE_HPP

#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"

namespace StraitX{
namespace Vk{

struct Fence: NonCopyable{
    VkFence Handle = VK_NULL_HANDLE;
    VkDevice Owner = VK_NULL_HANDLE;
    
    sx_inline void New(VkDevice dev){
        Owner = dev;

        VkFenceCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        CoreFunctionAssert(vkCreateFence(Owner, &info, nullptr, &Handle), VK_SUCCESS, "Vk: Fence: Creation Failed");
    }

    sx_inline void Delete(){
        vkDestroyFence(Owner, Handle, nullptr);
    }

    sx_inline void WaitFor()const{
        vkWaitForFences(Owner, 1, &Handle, true, ~0ull);
        vkResetFences(Owner, 1, &Handle);
    }
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_FENCE_HPP