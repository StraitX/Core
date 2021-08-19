#include "graphics/api/vulkan/command_buffer_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

CommandPoolImpl::CommandPoolImpl(QueueFamily::Type target_queue_family):
    m_TargetQueueFamily(target_queue_family),
    m_TargetQueueFamilyIndex(GPUImpl::s_Instance.QueueIndex(target_queue_family))
{
    VkCommandPoolCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    info.queueFamilyIndex = m_TargetQueueFamilyIndex;

    vkCreateCommandPool(GPUImpl::s_Instance.Handle(), &info, nullptr, &m_Handle);
}

CommandPoolImpl::~CommandPoolImpl(){
    vkDestroyCommandPool(GPUImpl::s_Instance.Handle(), m_Handle, nullptr);
}

CommandBuffer *CommandPoolImpl::Alloc(){
    return new CommandBufferImpl(m_Handle);
}

void CommandPoolImpl::Free(CommandBuffer *buffer){
    delete buffer;
}

CommandBufferImpl::CommandBufferImpl(VkCommandPool pool):
    m_Pool(pool)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = m_Pool;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = 1;

    vkAllocateCommandBuffers(GPUImpl::s_Instance.Handle(), &info, &m_Handle);
}

CommandBufferImpl::~CommandBufferImpl(){
    vkFreeCommandBuffers(GPUImpl::s_Instance.Handle(), m_Pool, 1, &m_Handle);
}

void CommandBufferImpl::Begin(){
    VkCommandBufferBeginInfo begin_info;
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    SX_VK_ASSERT(vkBeginCommandBuffer(m_Handle, &begin_info), "Vk: CommandBuffer: Failed to begin");
}

void CommandBufferImpl::End(){
    SX_VK_ASSERT(vkEndCommandBuffer(m_Handle), "Vk: CommandBuffer: Failed to end");
}

void CommandBufferImpl::Reset(){
    SX_VK_ASSERT(vkResetCommandBuffer(m_Handle, 0), "Vk: CommandBuffer: Failed to reset");
}

}//namespace Vk::