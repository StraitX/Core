#ifndef STRAITX_OPENGL_DMA_IMPL_HPP
#define STRAITX_OPENGL_DMA_IMPL_HPP

#include "graphics/api/dma.hpp"

namespace StraitX{
namespace GL{

struct DMAImpl{
    static void CopyMem2GPUBufferImpl(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset);

    static void CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    static void CopyCPU2GPUTextureImpl(const CPUTexture &src, const GPUTexture &dst);

    static void ChangeGPUTextureLayoutImpl(GPUTexture &src, GPUTexture::Layout layout);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_DMA_IMPL_HPP