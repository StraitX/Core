#include "graphics/vulkan/command_buffer.hpp"


namespace StraitX{
namespace Vk{

Error CommandBuffer::Create(Vk::CommandPool *pool){
    Pool = pool;
    VkCommandBufferAllocateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.pNext = nullptr;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandPool = Pool->Handle;
    info.commandBufferCount = 1;

    if(vkAllocateCommandBuffers(Pool->Owner->Handle, &info, &Handle) != VK_SUCCESS)
        return Error::Failure;
    return Error::Success;
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