#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"



namespace StraitX{
namespace Vk{

GPUContextImpl::GPUContextImpl(Vk::LogicalGPUImpl *owner):
    m_Owner(owner)
{
    VkCommandPoolCreateInfo pool_info;
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.pNext = nullptr;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = m_Owner->GraphicsQueue.FamilyIndex;

    CoreFunctionAssert(vkCreateCommandPool(m_Owner->Handle, &pool_info, nullptr, &m_CmdPool), VK_SUCCESS, "Vk: GPUContextImpl: Failed to create command pool");

    VkCommandBufferAllocateInfo buffer_info;
    buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_info.pNext = nullptr;
    buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    buffer_info.commandPool = m_CmdPool;
    buffer_info.commandBufferCount = 1;

    CoreFunctionAssert(vkAllocateCommandBuffers(m_Owner->Handle, &buffer_info, &m_CmdBuffer), VK_SUCCESS, "Vk: GPUContextImpl: Failed to allocate command buffer");
}


GPUContextImpl::~GPUContextImpl(){
    vkQueueWaitIdle(m_Owner->GraphicsQueue.Handle);

    vkFreeCommandBuffers(m_Owner->Handle, m_CmdPool, 1, &m_CmdBuffer);

    vkDestroyCommandPool(m_Owner->Handle, m_CmdPool, nullptr);
}

void GPUContextImpl::BeginFrame(){
    VkCommandBufferBeginInfo begin_info;
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    CoreFunctionAssert(vkBeginCommandBuffer(m_CmdBuffer, &begin_info), VK_SUCCESS, "Vk: GPUContext: Failed to begin CmdBuffer");
}

void GPUContextImpl::EndFrame(){
    (void)0;
}

void GPUContextImpl::Submit(){
    vkCmdPipelineBarrier(m_CmdBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 0, nullptr);

    SumbitAsync();
}

void GPUContextImpl::SumbitAsync(){
    CoreFunctionAssert(vkEndCommandBuffer(m_CmdBuffer),VK_SUCCESS, "Vk: GPUContext: Failed to end CmdBuffer for submission");

    m_Owner->SubmitCmdBuffer(m_Owner->GraphicsQueue, m_CmdBuffer);
}


GPUContext *GPUContextImpl::NewImpl(LogicalGPU &owner){
    return new(Memory::Alloc(sizeof(GPUContextImpl))) GPUContextImpl(static_cast<Vk::LogicalGPUImpl*>(&owner));
}

void GPUContextImpl::DeleteImpl(GPUContext *context){
    context->~GPUContext();
    Memory::Free(context);
}

}//namespace Vk::
}//namespace StraitX::