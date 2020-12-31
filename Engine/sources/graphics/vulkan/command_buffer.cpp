#include "graphics/vulkan/command_buffer.hpp"


namespace StraitX{
namespace Vk{

Result CommandBuffer::Create(Vk::CommandPool *pool){
    Pool = pool;
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandPool = Pool->Handle;
    info.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(Pool->Owner->Handle, &info, &Handle) != VK_SUCCESS)
        return Result::Failure;
    return Result::Success;
}

void CommandBuffer::Begin(){
    VkCommandBufferBeginInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext =nullptr;
    info.flags = 0;
    info.pInheritanceInfo = nullptr;
    vkBeginCommandBuffer(Handle, &info);
}


void CommandBuffer::BeginRenderTarget(Vk::GraphicsPipeline *pipeline, Vk::RenderPass *render_pass, Vk::Framebuffer *framebuffer, const ArrayPtr<VkClearValue> &clears){
    vkCmdBindPipeline(Handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->Handle);
    VkRenderPassBeginInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.renderArea = {{0,0}, {framebuffer->Size.x, framebuffer->Size.y}};
    info.renderPass = render_pass->Handle;
    info.framebuffer = framebuffer->Handle;
    info.clearValueCount = clears.Size;
    info.pClearValues = clears.Pointer;
    VkSubpassContents cont;
    vkCmdBeginRenderPass(Handle, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderTarget(){
    vkCmdEndRenderPass(Handle);
}

void CommandBuffer::End(){
    vkEndCommandBuffer(Handle);
}

void CommandBuffer::Reset(){
    vkResetCommandBuffer(Handle,0);
}

void CommandBuffer::Submit(const ArrayPtr<VkSemaphore> &wait_semaphores, const ArrayPtr<VkSemaphore> &signal_semaphores){
    VkSubmitInfo info;
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &Handle;
    info.waitSemaphoreCount = wait_semaphores.Size;
    info.pWaitSemaphores = wait_semaphores.Pointer;
    info.signalSemaphoreCount = signal_semaphores.Size;
    info.pSignalSemaphores = signal_semaphores.Pointer;
    info.pWaitDstStageMask = nullptr;

    vkQueueSubmit(Pool->Queue.Handle, 1, &info, VK_NULL_HANDLE);
}

void CommandBuffer::Destroy(){
    vkFreeCommandBuffers(Pool->Owner->Handle, Pool->Handle, 1, &Handle);
}


};//namespace Vk::
};//namespace StraitX::