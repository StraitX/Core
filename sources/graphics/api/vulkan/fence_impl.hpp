#ifndef STRAITX_VULKAN_FENCE_IMPL_HPP
#define STRAITX_VULKAN_FENCE_IMPL_HPP

#include "core/types.hpp"

namespace Vk{

struct FenceImpl{
    static u64 Create();

    static void Destroy(u64 handle);

    static void WaitFor(u64 handle);

    static void Reset(u64 handle);

    static bool IsSignaled(u64 handle);

    static void Signal(u64 handle);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_FENCE_IMPL_HPP