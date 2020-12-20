#ifndef STRAITX_VULKAN_COMMAND_BUFFER_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "graphics/vulkan/command_pool.hpp"


namespace StraitX{
namespace Vk{

struct CommandBuffer{
    VkCommandBuffer Handle = VK_NULL_HANDLE;
    Vk::CommandPool *Pool = nullptr;

    Result Create(Vk::CommandPool *pool);

    void Reset();

    void Submit(const ArrayPtr<VkSemaphore> &wait_semaphores, const ArrayPtr<VkSemaphore> &signal_semaphores);

    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_COMMAND_BUFFER_HPP