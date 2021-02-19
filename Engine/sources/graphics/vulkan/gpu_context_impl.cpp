#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"

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


void GPUContextImpl::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset){  
    CoreAssert(dst.Size() <= size + dst_offset, "Vk: GPUContext: Copy: Dst Buffer overflow");

    VkBufferCopy copy;
    copy.dstOffset = dst_offset;
    copy.srcOffset = 0;
    copy.size = size;

    vkCmdCopyBuffer(m_CmdBuffer, (VkBuffer)src.Handle().U64, (VkBuffer)dst.Handle().U64, 1, &copy);

    CmdMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
}

void GPUContextImpl::Bind(const GraphicsPipeline *pipeline){
    auto *pipeline_impl = static_cast<const Vk::GraphicsPipelineImpl *>(pipeline);
    vkCmdBindPipeline(m_CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_impl->Handle);
    
    vkCmdSetScissor(m_CmdBuffer, 0, 1, &pipeline_impl->Scissors);

    VkViewport viewport;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;
    viewport.x = pipeline_impl->Scissors.offset.x;
    viewport.y = pipeline_impl->Scissors.extent.height - pipeline_impl->Scissors.offset.y;
    viewport.width  = pipeline_impl->Scissors.extent.width;
    viewport.height = -(float)pipeline_impl->Scissors.extent.height;
    vkCmdSetViewport(m_CmdBuffer, 0, 1, &viewport);
}

void GPUContextImpl::BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer){
    const Vk::RenderPassImpl *pass_impl = static_cast<const Vk::RenderPassImpl*>(pass);
    const Vk::FramebufferImpl *fb_impl = static_cast<const Vk::FramebufferImpl*>(framebuffer);

    VkRenderPassBeginInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.clearValueCount = 0;
    info.pClearValues = nullptr;
    info.framebuffer = fb_impl->Handle;
    info.renderPass = pass_impl->Handle;
    info.renderArea.offset = {0, 0};
    info.renderArea.extent.width = framebuffer->Size().x;
    info.renderArea.extent.height = framebuffer->Size().y;
    
    vkCmdBeginRenderPass(m_CmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void GPUContextImpl::EndRenderPass(){
    vkCmdEndRenderPass(m_CmdBuffer);
}

void GPUContextImpl::BindVertexBuffer(const GPUBuffer &buffer){
    VkBuffer handle = reinterpret_cast<VkBuffer>(buffer.Handle().U64);
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(m_CmdBuffer, 0, 1, &handle, &offset);
}

void GPUContextImpl::BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type){
    VkBuffer handle = reinterpret_cast<VkBuffer>(buffer.Handle().U64);
    vkCmdBindIndexBuffer(m_CmdBuffer, handle, 0, VK_INDEX_TYPE_UINT32);
}

void GPUContextImpl::DrawIndexed(u32 indices_count){
    vkCmdDrawIndexed(m_CmdBuffer, indices_count, 1, 0, 0, 0);
}

void GPUContextImpl::CmdPipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst){
    vkCmdPipelineBarrier(m_CmdBuffer, src, dst, 0, 0, nullptr, 0, nullptr, 0, nullptr);
}

void GPUContextImpl::CmdMemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_access, VkAccessFlags dst_access){
    VkMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = src_access;
    barrier.dstAccessMask = dst_access;

    vkCmdPipelineBarrier(m_CmdBuffer, src, dst, 0, 1, &barrier, 0, nullptr, 0, nullptr);
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