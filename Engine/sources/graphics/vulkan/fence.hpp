#ifndef STRAITX_VULKAN_FENCE_HPP
#define STRAITX_VULKAN_FENCE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

struct Fence{
    VkFence Handle = VK_NULL_HANDLE;
    Vk::LogicalDevice *Owner = nullptr;

    void Create(Vk::LogicalDevice *owner);

    void WaitFor();

    void Reset();

    void Destroy();
};



sx_inline void Fence::WaitFor(){
    vkWaitForFences(Owner->Handle, 1, &Handle, true, ~0ull);
}

sx_inline void Fence::Reset(){
    vkResetFences(Owner->Handle, 1, &Handle);
}

sx_inline void Fence::Destroy(){
    vkDestroyFence(Owner->Handle, Handle, nullptr);
}



};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_FENCE_HPP