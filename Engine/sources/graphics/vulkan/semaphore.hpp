#ifndef STRAITX_VULKAN_SEMAPHORE_HPP
#define STRAITX_VULKAN_SEMAPHORE_HPP

#include "platform/vulkan.hpp"
#include "core/assert.hpp"

namespace Vk{

struct Semaphore{
    VkSemaphore Handle = VK_NULL_HANDLE;

    Semaphore();

    ~Semaphore();
};

}//Vk::

#endif//STRAITX_VULKAN_SEMAPHORE_HPP