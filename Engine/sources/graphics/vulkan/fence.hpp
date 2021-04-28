#ifndef STRAITX_VULKAN_FENCE_HPP
#define STRAITX_VULKAN_FENCE_HPP

#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"

namespace StraitX{
namespace Vk{

struct Fence: NonCopyable{
    VkFence Handle = VK_NULL_HANDLE;
    
    Fence();

    ~Fence();

    void WaitFor()const;
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_FENCE_HPP