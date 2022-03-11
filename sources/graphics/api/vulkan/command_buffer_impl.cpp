#include "graphics/api/vulkan/command_buffer_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/render_pass_impl.hpp"
#include "graphics/api/vulkan/framebuffer_impl.hpp"
#include "graphics/api/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/api/vulkan/buffer_impl.hpp"
#include "graphics/api/vulkan/descriptor_set_impl.hpp"

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

LayoutChangeOp *CommandBufferImpl::GetLastTextureLayoutChange(const Texture *texture){
    if(!m_Operations.Size())return nullptr;

    for(size_t i = m_Operations.Size()-1; i>=0; i--){
        if(m_Operations[i].Type == ResourceOperationType::LayoutChange){
            if(m_Operations[i].LayoutChange.Texture == texture)
                return &m_Operations[i].LayoutChange;
        }
    }
    return nullptr;
}

VkImageLayout CommandBufferImpl::GetExecutionTextureLayout(const Texture *texture){
    LayoutChangeOp *op = GetLastTextureLayoutChange(texture);

    return ToVkLayout(op ? op->NewLayout : texture->Layout());
}

void CommandBufferImpl::OnExecute(){
    for(const auto &op: m_Operations){
        if(op.Type == ResourceOperationType::LayoutChange)
            op.LayoutChange.Texture->m_Layout = op.LayoutChange.NewLayout;
    }
}

void CommandBufferImpl::Begin(){
    m_Operations.Clear();
    m_Bindings.Reset();

    VkCommandBufferBeginInfo begin_info;
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    SX_VK_ASSERT(vkBeginCommandBuffer(m_Handle, &begin_info), "Vk: CommandBuffer: Failed to begin");

    PipelineBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
}

void CommandBufferImpl::End(){
    PipelineBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);

    SX_VK_ASSERT(vkEndCommandBuffer(m_Handle), "Vk: CommandBuffer: Failed to end");
}

void CommandBufferImpl::Reset(){
    SX_VK_ASSERT(vkResetCommandBuffer(m_Handle, 0), "Vk: CommandBuffer: Failed to reset");
}

void CommandBufferImpl::PipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst)const{
    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 0, nullptr, 0, nullptr, 0, nullptr);
}

void CommandBufferImpl::MemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_access, VkAccessFlags dst_access)const{
    VkMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    barrier.pNext = nullptr;
    barrier.srcAccessMask = src_access;
    barrier.dstAccessMask = dst_access;

    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 1, &barrier, 0, nullptr, 0, nullptr);
}

void CommandBufferImpl::ImageBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, 
                                     VkAccessFlags src_acces, VkAccessFlags dst_access, 
                                     VkImageLayout old, VkImageLayout next, VkImage img, VkImageAspectFlags aspect)const
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
    barrier.subresourceRange.aspectMask = aspect;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(m_Handle, src, dst, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void CommandBufferImpl::ChangeLayout(const Texture2D *texture, TextureLayout new_layout){

    ImageBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        GetExecutionTextureLayout(texture), 
        ToVkLayout(new_layout), 
        *(Vk::Texture2DImpl*)texture,
        IsDepthFormat(texture->Format()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT
    );

    m_Operations.Add({texture, new_layout});
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

void CommandBufferImpl::Bind(const GraphicsPipeline *pipeline){
    const Pipeline *impl = (const Vk::GraphicsPipelineImpl*)pipeline;
    m_Bindings.Bind(impl);

    vkCmdBindPipeline(m_Handle, impl->BindPoint(), *impl);
}

void CommandBufferImpl::Draw(u32 vertices_count){
    vkCmdDraw(m_Handle, vertices_count, 1, 0, 0);
}
void CommandBufferImpl::Bind(const ComputePipeline* pipeline) {
    const Pipeline *impl = (const Vk::ComputePipelineImpl*)pipeline;
    m_Bindings.Bind(impl);

    vkCmdBindPipeline(m_Handle, impl->BindPoint(), *impl);
}

void CommandBufferImpl::Dispatch(u32 group_size_x, u32 group_size_y, u32 group_size_z) {
    vkCmdDispatch(m_Handle, group_size_x, group_size_y, group_size_z);
}

void CommandBufferImpl::SetScissor(s32 x, s32 y, u32 width, u32 height){
    VkRect2D scissor;
    scissor.offset.x = x;
    scissor.offset.y = y;
    scissor.extent.width = width;
    scissor.extent.height = height;
    vkCmdSetScissor(m_Handle, 0, 1, &scissor);
}

void CommandBufferImpl::SetViewport(s32 x, s32 y, u32 width, u32 height){
    VkViewport viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    vkCmdSetViewport(m_Handle, 0, 1, &viewport);
}

void CommandBufferImpl::SetLineWidth(u32 width) {
    vkCmdSetLineWidth(m_Handle, width);
}

void CommandBufferImpl::BindVertexBuffer(const Buffer *buffer){
    VkBuffer handle = *(const Vk::BufferImpl*)buffer;
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(m_Handle, 0, 1, &handle, &offset);
}

void CommandBufferImpl::BindIndexBuffer(const Buffer *buffer, IndicesType type){
    static const VkIndexType s_IndicesType[]={
        VK_INDEX_TYPE_UINT16,
        VK_INDEX_TYPE_UINT32,
    };

    vkCmdBindIndexBuffer(m_Handle, *(const Vk::BufferImpl*)buffer, 0, s_IndicesType[(size_t)type]);
}

void CommandBufferImpl::DrawIndexed(u32 indices_count, u32 indices_offset){
    vkCmdDrawIndexed(m_Handle, indices_count, 1, indices_offset, 0, 0);
}

void CommandBufferImpl::Copy(const Buffer *src, const Buffer *dst, size_t size, size_t src_offset, size_t dst_offset){
    MemoryBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, 
        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT
    );

    VkBufferCopy region;
    region.srcOffset = src_offset;
    region.dstOffset = dst_offset;
    region.size = size;

    vkCmdCopyBuffer(m_Handle, *(const Vk::BufferImpl*)src, *(const Vk::BufferImpl*)dst, 1, &region);

    MemoryBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT, 
        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT
    );
}

void CommandBufferImpl::Copy(const Buffer *src, const Texture2D *dst, Vector2u src_size, Vector2u dst_offset){
    VkImageLayout current_layout = GetExecutionTextureLayout(dst);
    TextureLayout copy_layout = TextureLayout::TransferDstOptimal;
    VkImageAspectFlags aspect = IsDepthFormat(dst->Format()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;


    ImageBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        current_layout, 
        ToVkLayout(copy_layout), 
        *(Vk::Texture2DImpl*)dst,
        aspect
    );

    VkBufferImageCopy copy;
    copy.bufferImageHeight = src_size.y;
    copy.bufferOffset = 0;
    copy.bufferRowLength = src_size.x;
    copy.imageOffset.x = dst_offset.x;
    copy.imageOffset.y = dst_offset.y;
    copy.imageOffset.z = 0;
    copy.imageExtent.depth = 1;
    copy.imageExtent.width = dst->Size().x;
    copy.imageExtent.height = dst->Size().y;
    copy.imageSubresource.aspectMask = aspect;
    copy.imageSubresource.baseArrayLayer = 0;
    copy.imageSubresource.layerCount = 1;
    copy.imageSubresource.mipLevel = 0;
    
    vkCmdCopyBufferToImage(m_Handle, *(const Vk::BufferImpl*)src, *(const Vk::Texture2DImpl*)dst, ToVkLayout(copy_layout), 1, &copy);

    if(current_layout != VK_IMAGE_LAYOUT_UNDEFINED){
        ImageBarrier(
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            ToVkLayout(copy_layout), 
            current_layout, 
            *(Vk::Texture2DImpl*)dst,
            aspect
        );
    }else{
        m_Operations.Add({dst, TextureLayout::TransferDstOptimal});
    }
}

void CommandBufferImpl::ClearColor(const Texture2D *texture, const Color &color){
    VkImageLayout current_layout = GetExecutionTextureLayout(texture);
    TextureLayout clear_layout = TextureLayout::TransferDstOptimal;

    ImageBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        current_layout, 
        ToVkLayout(clear_layout), 
        *(Vk::Texture2DImpl*)texture,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    VkClearColorValue value;
    value.float32[0] = color.R;
    value.float32[1] = color.G;
    value.float32[2] = color.B;
    value.float32[3] = color.A;

    VkImageSubresourceRange range;
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;

    vkCmdClearColorImage(m_Handle, *(Vk::Texture2DImpl*)texture, ToVkLayout(clear_layout), &value, 1, &range);

    if(current_layout != VK_IMAGE_LAYOUT_UNDEFINED){
        ImageBarrier(
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            ToVkLayout(clear_layout), 
            current_layout, 
            *(Vk::Texture2DImpl*)texture,
            VK_IMAGE_ASPECT_COLOR_BIT
        );
    }else{
        m_Operations.Add({texture, clear_layout});
    }
}

void CommandBufferImpl::ClearDepthStencil(const Texture2D *texture, float depth, u8 stencil){
    VkImageLayout current_layout = GetExecutionTextureLayout(texture);
    TextureLayout clear_layout = TextureLayout::TransferDstOptimal;
    VkImageAspectFlags aspect = 0;
    
    if(IsDepthFormat(texture->Format()))
        aspect |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if(IsStencilFormat(texture->Format()))
        aspect |= VK_IMAGE_ASPECT_STENCIL_BIT;

    ImageBarrier(
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
        current_layout, 
        ToVkLayout(clear_layout), 
        *(Vk::Texture2DImpl*)texture,
        aspect
    );

    VkClearDepthStencilValue value;
    value.depth = depth;
    value.stencil = stencil;

    VkImageSubresourceRange range;
    range.aspectMask = aspect;
    range.baseMipLevel = 0;
    range.levelCount = 1;
    range.baseArrayLayer = 0;
    range.layerCount = 1;

    vkCmdClearDepthStencilImage(m_Handle, *(Vk::Texture2DImpl*)texture, ToVkLayout(clear_layout), &value, 1, &range);

    if(current_layout != VK_IMAGE_LAYOUT_UNDEFINED){
        ImageBarrier(
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
            ToVkLayout(clear_layout), 
            current_layout, 
            *(Vk::Texture2DImpl*)texture,
            aspect
        );
    }else{
        m_Operations.Add({texture, clear_layout});
    }
}

void CommandBufferImpl::Bind(const DescriptorSet *set){
    VkDescriptorSet handle = *(const Vk::DescriptorSetImpl*)set;
    
    vkCmdBindDescriptorSets(m_Handle, m_Bindings.PipelineBinding->BindPoint(), m_Bindings.PipelineBinding->Layout(), 0, 1, &handle, 0, nullptr);
}

}//namespace Vk::