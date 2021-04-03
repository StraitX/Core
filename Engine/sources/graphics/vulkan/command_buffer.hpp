#ifndef STRAITX_VULKAN_COMMAND_BUFFER_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_HPP

#include "platform/vulkan.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

struct CommandBuffer{
    VkDevice Owner = VK_NULL_HANDLE;
    Vk::Queue Queue; 
    VkCommandPool Pool = VK_NULL_HANDLE;
    VkCommandBuffer Handle = VK_NULL_HANDLE;

    void New(VkDevice owner, Vk::Queue queue);

    void Delete();

    void Begin()const;

    void End()const;

    void Submit(const ArrayPtr<const VkSemaphore> &wait_semaphores, const ArrayPtr<const VkSemaphore> &signal_semaphores, VkFence signal_fence)const;

    void CmdPipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst);

    void CmdMemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access);

    void CmdImageBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access, VkImageLayout old, VkImageLayout next, VkImage img);

    operator VkCommandBuffer()const;
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_COMMAND_BUFFER_HPP