#include "graphics/vulkan/semaphore.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/debug.hpp"

namespace Vk{

Semaphore::Semaphore()
{
    VkSemaphoreCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;

    SX_VK_ASSERT(vkCreateSemaphore(GPU::Get().Handle(), &info, nullptr, &Handle), "Vk: Can't create semaphore");
}

Semaphore::~Semaphore(){
    vkDestroySemaphore(GPU::Get().Handle(), Handle, nullptr);
}

}// namespace Vk::