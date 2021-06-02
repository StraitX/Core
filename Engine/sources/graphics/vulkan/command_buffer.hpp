#ifndef STRAITX_VULKAN_COMMAND_BUFFER_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_HPP

#include "platform/vulkan.hpp"
#include "core/span.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

class CommandBuffer{
private:
    QueueFamily::Type m_TargetQueueType;
    VkQueue m_TargetQueue = VK_NULL_HANDLE;
    VkCommandPool m_Pool = VK_NULL_HANDLE;
    VkCommandBuffer m_Handle = VK_NULL_HANDLE;
public: 
    CommandBuffer(QueueFamily::Type target_queue_type);

    ~CommandBuffer();

    void Begin()const;

    void End()const;

    void Submit(const Span<VkSemaphore> &wait_semaphores, const Span<VkSemaphore> &signal_semaphores, VkFence signal_fence)const;

    void CmdBufferCopy(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize src_offset, VkDeviceSize dst_offset)const;

    void CmdPipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst)const;

    void CmdMemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access)const;

    void CmdImageBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access, VkImageLayout old, VkImageLayout next, VkImage img, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT)const;

    operator VkCommandBuffer()const;

    VkQueue TargetQueue()const{
        return m_TargetQueue;
    }
};

SX_INLINE void CommandBuffer::CmdBufferCopy(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize src_offset, VkDeviceSize dst_offset)const{
    VkBufferCopy copy;
    copy.srcOffset = src_offset;
    copy.dstOffset = dst_offset;
    copy.size = size;
    vkCmdCopyBuffer(m_Handle, src, dst, 1, &copy);
}

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_COMMAND_BUFFER_HPP