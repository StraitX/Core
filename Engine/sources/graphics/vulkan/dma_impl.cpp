#include "graphics/vulkan/dma_impl.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"

namespace StraitX{
namespace Vk{

void DMAImpl::CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
    auto gpu = static_cast<const Vk::LogicalGPUImpl*>(dst.Owner());

    gpu->TransferCmdBuffer.Begin();
    {
        VkBufferCopy copy;
        copy.srcOffset = src_offset;
        copy.dstOffset = dst_offset;
        copy.size = size;
        vkCmdCopyBuffer(gpu->TransferCmdBuffer, (VkBuffer)src.Handle().U64, (VkBuffer)dst.Handle().U64, 1, &copy);
    }
    gpu->TransferCmdBuffer.End();

    gpu->TransferCmdBuffer.Submit({nullptr, 0}, {nullptr, 0}, gpu->TransferFence.Handle);
    gpu->TransferFence.WaitFor();
}

void DMAImpl::CopyCPU2GPUTextureImpl(const CPUTexture &src, const GPUTexture &dst){
    auto gpu = static_cast<const Vk::LogicalGPUImpl*>(dst.Owner());

    gpu->TransferCmdBuffer.Begin();
    {
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
        vkCmdCopyBufferToImage(gpu->TransferCmdBuffer, (VkBuffer)src.Handle().U64, (VkImage)dst.Handle().U64, GPUTextureImpl::s_LayoutTable[(size_t)dst.GetLayout()], 1, &copy);
    }
    gpu->TransferCmdBuffer.End();

    gpu->TransferCmdBuffer.Submit({nullptr, 0}, {nullptr, 0}, gpu->TransferFence.Handle);
    gpu->TransferFence.WaitFor();
}

void DMAImpl::ChangeGPUTextureLayoutImpl(GPUTexture &src, GPUTexture::Layout layout){
    auto gpu = static_cast<const Vk::LogicalGPUImpl*>(src.Owner());

    gpu->TransferCmdBuffer.Begin();
    {
        VkImageMemoryBarrier barrier;
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext = nullptr;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.oldLayout = GPUTextureImpl::s_LayoutTable[(size_t)src.GetLayout()];
        barrier.newLayout = GPUTextureImpl::s_LayoutTable[(size_t)layout];
        barrier.srcQueueFamilyIndex = 0;
        barrier.dstQueueFamilyIndex = 0;
        barrier.image = (VkImage)src.Handle().U64;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(gpu->TransferCmdBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }
    gpu->TransferCmdBuffer.End();

    gpu->TransferCmdBuffer.Submit({nullptr, 0}, {nullptr, 0}, gpu->TransferFence.Handle);
    gpu->TransferFence.WaitFor();

    src.m_Layout = layout;
}

}//namespace Vk::
}//namespace StraitX::