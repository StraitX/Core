#include "graphics/api/vulkan/semaphore_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"

namespace Vk{

u64 SemaphoreImpl::Create(){
    VkSemaphoreCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;

    VkSemaphore semaphore = VK_NULL_HANDLE;
    vkCreateSemaphore(GPUImpl::s_Instance.Handle(), &info, nullptr, &semaphore);

    return reinterpret_cast<u64>(semaphore);
}

void SemaphoreImpl::Destroy(u64 handle){
    VkSemaphore semaphore = reinterpret_cast<VkSemaphore>(handle);
    vkDestroySemaphore(GPUImpl::s_Instance.Handle(), semaphore, nullptr);
}

}//namespace Vk::