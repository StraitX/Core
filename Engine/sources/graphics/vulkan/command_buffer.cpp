#include "platform/memory.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/command_buffer.hpp"

namespace StraitX{
namespace Vk{

CommandBuffer::CommandBuffer(QueueFamily::Type target_queue_type):
    m_TargetQueueType(target_queue_type),
    m_TargetQueue(GPU::Get().Queue(m_TargetQueueType))
{

    VkCommandPoolCreateInfo pool_info;
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.pNext = nullptr;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = GPU::Get().QueueIndex(m_TargetQueueType);

    CoreFunctionAssert(vkCreateCommandPool(GPU::Get().Handle(), &pool_info, nullptr, &m_Pool), VK_SUCCESS, "Vk: CommandBuffer: Failed to create command pool");

    VkCommandBufferAllocateInfo buffer_info;
    buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_info.pNext = nullptr;
    buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    buffer_info.commandPool = m_Pool;
    buffer_info.commandBufferCount = 1;

    CoreFunctionAssert(vkAllocateCommandBuffers(GPU::Get().Handle(), &buffer_info, &m_Handle), VK_SUCCESS, "Vk: CommandBuffer: Failed to allocate");
}

CommandBuffer::~CommandBuffer(){
    vkQueueWaitIdle(m_TargetQueue);

    vkFreeCommandBuffers(GPU::Get().Handle(), m_Pool, 1, &m_Handle);

    vkDestroyCommandPool(GPU::Get().Handle(), m_Pool, nullptr);
}

void CommandBuffer::Begin()const{
    VkCommandBufferBeginInfo begin_info;
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    CoreFunctionAssert(vkBeginCommandBuffer(m_Handle, &begin_info), VK_SUCCESS, "Vk: CommandBuffer: Failed to begin");
}

void CommandBuffer::End()const{
    CoreFunctionAssert(vkEndCommandBuffer(m_Handle),VK_SUCCESS, "Vk: CommandBuffer: Failed to end");
}

void CommandBuffer::Submit(const ArrayPtr<const VkSemaphore> &wait_semaphores, const ArrayPtr<const VkSemaphore> &signal_semaphores, VkFence signal_fence)const{
    auto *stages = (VkPipelineStageFlags*)alloca(wait_semaphores.Size() * sizeof(VkPipelineStageFlags));

    for(size_t i = 0; i<wait_semaphores.Size(); ++i){
        stages[i] = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }

    VkSubmitInfo info;
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &m_Handle;
    info.waitSemaphoreCount = wait_semaphores.Size();
    info.pWaitSemaphores = wait_semaphores.Pointer();
    info.signalSemaphoreCount = signal_semaphores.Size();
    info.pSignalSemaphores = signal_semaphores.Pointer();
    info.pWaitDstStageMask = stages;

    CoreFunctionAssert(vkQueueSubmit(m_TargetQueue, 1, &info, signal_fence), VK_SUCCESS, "Vk: CommandBuffer: Failed to submit");
}

void CommandBuffer::CmdPipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst)const{
    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 0, nullptr, 0, nullptr, 0, nullptr);
}

void CommandBuffer::CmdMemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_access, VkAccessFlags dst_access)const{
    VkMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = src_access;
    barrier.dstAccessMask = dst_access;

    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 1, &barrier, 0, nullptr, 0, nullptr);
}

void CommandBuffer::CmdImageBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, 
                                     VkAccessFlags src_acces, VkAccessFlags dst_access, 
                                     VkImageLayout old, VkImageLayout next, VkImage img)const
{
    VkImageMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = src_acces;
    barrier.dstAccessMask = dst_access;
    barrier.oldLayout = old;
    barrier.newLayout = next;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = img;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

CommandBuffer::operator VkCommandBuffer()const{
    return m_Handle;
}

}//namespace Vk::
}//namespace StraitX::