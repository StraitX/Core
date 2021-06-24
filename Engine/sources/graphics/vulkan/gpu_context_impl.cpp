#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/descriptor_set_impl.hpp"

namespace StraitX{
namespace Vk{
VkIndexType GPUContextImpl::s_IndexTypeTable[]={
    VK_INDEX_TYPE_UINT16,
    VK_INDEX_TYPE_UINT32
};

constexpr size_t GPUContextImpl::SemaphoreRingSize;

GPUContextImpl::GPUContextImpl(){

    { // XXX: Signal first semaphore to avoid lock
        m_CmdBuffer.Begin();
        m_CmdBuffer.End();
        m_CmdBuffer.Submit(Span<VkSemaphore>(), Span<VkSemaphore>(&m_SemaphoreRing[0].Handle, 1), VK_NULL_HANDLE);
        vkQueueWaitIdle(m_CmdBuffer.TargetQueue());
    }
}

void GPUContextImpl::BeginImpl(){
    m_CmdBuffer.Begin();
}

void GPUContextImpl::EndImpl(){
    m_CmdBuffer.End();

	m_Pipeline = nullptr;
}

void GPUContextImpl::SubmitImpl(){
    auto semaphores = NextPair();

    m_CmdBuffer.Submit(Span<VkSemaphore>(&semaphores.First, 1), Span<VkSemaphore>(&semaphores.Second, 1), VK_NULL_HANDLE);

    vkQueueWaitIdle(m_CmdBuffer.TargetQueue()); // TODO Get rid of this // Context is Immediate mode for now :'-
}

void GPUContextImpl::CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){  
    SX_CORE_ASSERT(size + dst_offset <= dst.Size(), "Vk: GPUContext: Copy: Dst Buffer overflow");

    m_CmdBuffer.CmdBufferCopy(
        (VkBuffer)src.Handle().U64, 
        (VkBuffer)dst.Handle().U64, 
        size, 
        src_offset, 
        dst_offset
    );

    m_CmdBuffer.CmdMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
}

void GPUContextImpl::CopyImpl(const CPUTexture &src, const GPUTexture &dst){
    VkBufferImageCopy copy;
    copy.bufferImageHeight = src.Size().y;
    copy.bufferOffset = 0;
    copy.bufferRowLength = src.Size().x;
    copy.imageOffset = {};
    copy.imageExtent.depth = 1;
    copy.imageExtent.width = dst.Size().x;
    copy.imageExtent.height = dst.Size().y;
    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.imageSubresource.baseArrayLayer = 0;
    copy.imageSubresource.layerCount = 1;
    copy.imageSubresource.mipLevel = 0;
    vkCmdCopyBufferToImage(m_CmdBuffer, (VkBuffer)src.Handle().U64, (VkImage)dst.Handle().U64, GPUTextureImpl::s_LayoutTable[(size_t)dst.GetLayout()], 1, &copy);
}

void GPUContextImpl::ChangeLayoutImpl(GPUTexture &texture, GPUTexture::Layout new_layout){

    m_CmdBuffer.CmdImageBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        GPUTextureImpl::s_LayoutTable[(size_t)texture.GetLayout()], 
        GPUTextureImpl::s_LayoutTable[(size_t)new_layout], 
        (VkImage)texture.Handle().U64
    );

    texture.m_Layout = new_layout;
}

void GPUContextImpl::BindImpl(const GraphicsPipeline *pipeline){
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

	m_Pipeline = pipeline_impl;
}

void GPUContextImpl::BindDescriptorSetImpl(const DescriptorSet *set){
	VkDescriptorSet set_handle = ((const DescriptorSetImpl *)set)->Handle();

    vkCmdBindDescriptorSets(m_CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->Layout, 0, 1, &set_handle, 0, nullptr);
}

void GPUContextImpl::BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer){
    m_RenderPass = static_cast<const Vk::RenderPassImpl*>(pass);
    m_Framebuffer = static_cast<const Vk::FramebufferImpl*>(framebuffer);

    VkRenderPassBeginInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.pNext = nullptr;
    info.clearValueCount = 0;
    info.pClearValues = nullptr;
    info.framebuffer = m_Framebuffer->Handle();
    info.renderPass = m_RenderPass->Handle();
    info.renderArea.offset = {0, 0};
    info.renderArea.extent.width = framebuffer->Size().x;
    info.renderArea.extent.height = framebuffer->Size().y;
    
    vkCmdBeginRenderPass(m_CmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void GPUContextImpl::EndRenderPassImpl(){
    vkCmdEndRenderPass(m_CmdBuffer);
    m_RenderPass = nullptr;
    m_Framebuffer = nullptr;
}

void GPUContextImpl::BindVertexBufferImpl(const GPUBuffer &buffer){
    VkBuffer handle = reinterpret_cast<VkBuffer>(buffer.Handle().U64);
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(m_CmdBuffer, 0, 1, &handle, &offset);
}

void GPUContextImpl::BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type){
    VkBuffer handle = reinterpret_cast<VkBuffer>(buffer.Handle().U64);
    vkCmdBindIndexBuffer(m_CmdBuffer, handle, 0, s_IndexTypeTable[(size_t)indices_type]);
}

void GPUContextImpl::DrawIndexedImpl(u32 indices_count){
    vkCmdDrawIndexed(m_CmdBuffer, indices_count, 1, 0, 0, 0);
}

Pair<VkSemaphore, VkSemaphore> GPUContextImpl::NextPair(){
    Pair<VkSemaphore, VkSemaphore> result = {m_SemaphoreRing[m_SemaphoreRingCounter % SemaphoreRingSize].Handle, m_SemaphoreRing[(m_SemaphoreRingCounter + 1) % SemaphoreRingSize].Handle};
    ++m_SemaphoreRingCounter;
    return result;
}

void GPUContextImpl::ClearFramebufferColorAttachmentsImpl(const Framebuffer *fb, const Vector4f &color){
    auto fb_impl = static_cast<const Vk::FramebufferImpl*>(fb);
    SX_CORE_ASSERT(m_Framebuffer != fb_impl, "Vk: GPUContextImpl: can't clear framebuffer which is being used in current render pass");

    VkClearColorValue value;
    value.float32[0] = color[0];
    value.float32[1] = color[1];
    value.float32[2] = color[2];
    value.float32[3] = color[3];

    VkImageSubresourceRange issr;
    issr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    issr.baseArrayLayer = 0;
    issr.baseMipLevel = 0;
    issr.levelCount = 1;
    issr.layerCount = 1;

    for(auto &att: fb_impl->Attachments()){
        if(IsColorFormat(att->GetFormat())){

            auto layout = GPUTextureImpl::s_LayoutTable[(size_t)att->GetLayout()];
            constexpr auto clear_layout = VK_IMAGE_LAYOUT_GENERAL;

            m_CmdBuffer.CmdImageBarrier(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, layout, clear_layout, (VkImage)att->Handle().U64);

            vkCmdClearColorImage(m_CmdBuffer, (VkImage)att->Handle().U64, clear_layout, &value, 1, &issr);

            m_CmdBuffer.CmdImageBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_READ_BIT, clear_layout, layout, (VkImage)att->Handle().U64);
        }
    }
}

void GPUContextImpl::ClearFramebufferDepthAttachmentsImpl(const Framebuffer *fb, float value){
    auto fb_impl = static_cast<const Vk::FramebufferImpl*>(fb);
    SX_CORE_ASSERT(m_Framebuffer != fb_impl, "Vk: GPUContextImpl: can't clear framebuffer which is being used in current render pass");

    VkClearDepthStencilValue depth_value;
    depth_value.depth = value;
    depth_value.stencil = 0;

    VkImageSubresourceRange issr;
    issr.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    issr.baseArrayLayer = 0;
    issr.baseMipLevel = 0;
    issr.levelCount = 1;
    issr.layerCount = 1;

    for(auto &att: fb_impl->Attachments()){
        if(IsDepthFormat(att->GetFormat())){

            auto layout = GPUTextureImpl::s_LayoutTable[(size_t)att->GetLayout()];
            constexpr auto clear_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

            m_CmdBuffer.CmdImageBarrier(VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, layout, clear_layout, (VkImage)att->Handle().U64, VK_IMAGE_ASPECT_DEPTH_BIT);

            vkCmdClearDepthStencilImage(m_CmdBuffer, (VkImage)att->Handle().U64, clear_layout, &depth_value, 1, &issr);

            m_CmdBuffer.CmdImageBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_READ_BIT, clear_layout, layout, (VkImage)att->Handle().U64, VK_IMAGE_ASPECT_DEPTH_BIT);
        }
    }
}

void GPUContextImpl::SwapFramebuffersImpl(Swapchain *swapchain){
    auto *swapchain_impl = static_cast<Vk::SwapchainImpl*>(swapchain);
    auto semaphores = NextPair();

    swapchain_impl->PresentCurrent(semaphores.First);
    swapchain_impl->AcquireNext(semaphores.Second);
}

GPUContext *GPUContextImpl::NewImpl(){
    return new(Memory::Alloc(sizeof(GPUContextImpl))) GPUContextImpl();
}

void GPUContextImpl::DeleteImpl(GPUContext *context){
    context->~GPUContext();
    Memory::Free(context);
}

}//namespace Vk::
}//namespace StraitX::