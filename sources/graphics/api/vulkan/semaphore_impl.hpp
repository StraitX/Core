#ifndef STRAITX_VULKAN_SEMAPHORE_IMPL_HPP
#define STRAITX_VULKAN_SEMAPHORE_IMPL_HPP

#include "graphics/api/semaphore.hpp"

namespace Vk{

struct SemaphoreImpl{
    static u64 Create();

    static void Destroy(u64 handle);

    static void WaitFor(const Semaphore &sempahore);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_SEMAPHORE_IMPL_HPP