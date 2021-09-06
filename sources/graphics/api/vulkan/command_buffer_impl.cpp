#include "graphics/api/vulkan/command_buffer_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/render_pass_impl.hpp"
#include "graphics/api/vulkan/framebuffer_impl.hpp"

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

    vkCreateCommandPool(GPUImpl::s_Instance, &info, nullptr, &m_Handle);
}

CommandPoolImpl::~CommandPoolImpl(){
    vkDestroyCommandPool(GPUImpl::s_Instance, m_Handle, nullptr);
}

CommandBuffer *CommandPoolImpl::Alloc(){
    return new CommandBufferImpl(this);
}

void CommandPoolImpl::Free(CommandBuffer *buffer){
    delete buffer;
}

CommandBufferImpl::CommandBufferImpl(CommandPoolImpl *pool):
    m_Pool(pool)
{
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.commandPool = *m_Pool;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = 1;

    vkAllocateCommandBuffers(GPUImpl::s_Instance, &info, &m_Handle);
}

CommandBufferImpl::~CommandBufferImpl(){
    vkFreeCommandBuffers(GPUImpl::s_Instance, *m_Pool, 1, &m_Handle);
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

void CommandBufferImpl::ChangeLayout(Texture2D *texture, TextureLayout new_layout){
    VkImageMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT;
    barrier.oldLayout = ToVkLayout(texture->Layout());
    barrier.newLayout = ToVkLayout(new_layout);
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = *(Vk::Texture2DImpl*)texture;
    barrier.subresourceRange.aspectMask = IsDepthFormat(texture->Format()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(m_Handle, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void CommandBufferImpl::BeginRenderPass(const RenderPass *rp, const Framebuffer *fb){
    VkRenderPassBeginInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.renderPass = *(const Vk::RenderPassImpl*)rp;
    info.framebuffer = *(const Vk::FramebufferImpl*)fb;
    info.renderArea.offset = {0, 0};
    info.renderArea.extent = {fb->Size().x, fb->Size().y};
    info.clearValueCount = 0;
    info.pClearValues = nullptr;
    
    vkCmdBeginRenderPass(m_Handle, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBufferImpl::EndRenderPass(){
    vkCmdEndRenderPass(m_Handle);
}

}//namespace Vk::