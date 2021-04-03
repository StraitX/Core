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
        gpu->TransferCmdBuffer.CmdBufferCopy(
            (VkBuffer)src.Handle().U64, 
            (VkBuffer)dst.Handle().U64, 
            size, 
            src_offset, 
            dst_offset
        );
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
        gpu->TransferCmdBuffer.CmdImageBarrier(
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_ACCESS_MEMORY_WRITE_BIT, 
            VK_ACCESS_MEMORY_READ_BIT, 
            GPUTextureImpl::s_LayoutTable[(size_t)src.GetLayout()], 
            GPUTextureImpl::s_LayoutTable[(size_t)layout], 
            (VkImage)src.Handle().U64
        );
    }
    gpu->TransferCmdBuffer.End();

    gpu->TransferCmdBuffer.Submit({nullptr, 0}, {nullptr, 0}, gpu->TransferFence.Handle);
    gpu->TransferFence.WaitFor();

    src.m_Layout = layout;
}

}//namespace Vk::
}//namespace StraitX::