#ifndef STRAITX_VULKAN_DMA_IMPL_HPP
#define STRAITX_VULKAN_DMA_IMPL_HPP

#include "graphics/api/dma.hpp"

namespace StraitX{
namespace Vk{

struct DMAImpl{
    static void CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset);

    static void CopyCPU2GPUTextureImpl(const CPUTexture &src, const GPUTexture &dst);

    static void ChangeGPUTextureLayoutImpl(GPUTexture &src, GPUTexture::Layout layout);

};

}//namespace VK::
}//namespace StraitX::

#endif//STRAITX_VULKAN_DMA_IMPL_HPP