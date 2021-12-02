#include "core/os/vulkan.hpp"
#include "graphics/api/vulkan/fence_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/immediate.hpp"

namespace Vk{

u64 FenceImpl::Create(){
    VkFenceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;

    VkFence fence = VK_NULL_HANDLE;

    vkCreateFence(GPUImpl::s_Instance, &info, nullptr, &fence);

    return reinterpret_cast<u64>(fence);
}

void FenceImpl::Destroy(u64 handle){
    VkFence fence = reinterpret_cast<VkFence>(handle);

    vkDestroyFence(GPUImpl::s_Instance, fence, nullptr);
}

void FenceImpl::WaitFor(u64 handle){
    VkFence fence = reinterpret_cast<VkFence>(handle);

    vkWaitForFences(GPUImpl::s_Instance, 1, &fence, true, u64(-1));
}

void FenceImpl::Reset(u64 handle){
    VkFence fence = reinterpret_cast<VkFence>(handle);

    vkResetFences(GPUImpl::s_Instance, 1, &fence);
}

bool FenceImpl::IsSignaled(u64 handle) {
    VkFence fence = reinterpret_cast<VkFence>(handle);

    return vkGetFenceStatus(GPUImpl::s_Instance, fence) == VK_SUCCESS;
}

void FenceImpl::Signal(u64 handle){
    Immediate::SignalFence(handle);
}

}//namespace Vk::