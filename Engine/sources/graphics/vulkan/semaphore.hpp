#ifndef STRAITX_VULKAN_SEMAPHORE_HPP
#define STRAITX_VULKAN_SEMAPHORE_HPP

#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

struct Semaphore{
    VkSemaphore Handle = VK_NULL_HANDLE;

    sx_inline Semaphore();

    sx_inline ~Semaphore();
};

sx_inline Semaphore::Semaphore()
{
    VkSemaphoreCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;

    CoreFunctionAssert(vkCreateSemaphore(GPU::Get().Handle(), &info, nullptr, &Handle),VK_SUCCESS, "Vk: Can't create semaphore");
}

sx_inline Semaphore::~Semaphore(){
    vkDestroySemaphore(GPU::Get().Handle(), Handle, nullptr);
}

}//Vk::
}//StraitX::

#endif//STRAITX_VULKAN_SEMAPHORE_HPP