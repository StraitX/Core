#ifndef STRAITX_DMA_HPP
#define STRAITX_DMA_HPP

#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/cpu_texture.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{

class GraphicsAPILoader;

class DMA{
public:
    struct VTable{
        using CopyMem2GPUBufferProc = void (*)(const void *, const GPUBuffer &dst, u32 size, u32 dst_offset);
        using CopyCPU2GPUBufferProc = void (*)(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);
        using CopyCPU2GPUTextureProc = void (*)(const CPUTexture &src, const GPUTexture &dst);
        using ChangeGPUTextureLayoutProc = void (*)(GPUTexture &texture, GPUTexture::Layout layout);

        CopyMem2GPUBufferProc  CopyMem2GPUBuffer  = nullptr;
        CopyCPU2GPUBufferProc  CopyCPU2GPUBuffer  = nullptr;
        CopyCPU2GPUTextureProc CopyCPU2GPUTexture = nullptr;
        ChangeGPUTextureLayoutProc ChangeGPUTextureLayout = nullptr;
    };
private:
    static VTable s_VTable;

    friend class GraphicsAPILoader;
public:

    static void Copy(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0);

    static void Copy(const void *src, const GPUBuffer &dst);

    static void Copy(const CPUBuffer &src, const GPUBuffer &dst);

    static void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset = 0, u32 dst_offset = 0);

    static void Copy(const CPUTexture &src, const GPUTexture &dst);

    static void ChangeLayout(GPUTexture &texture, GPUTexture::Layout layout);
};

sx_inline void DMA::Copy(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    s_VTable.CopyMem2GPUBuffer(src, dst, size, dst_offset);
}

sx_inline void DMA::Copy(const void *src, const GPUBuffer &dst){
    Copy(src, dst, dst.Size(), 0);
}

sx_inline void DMA::Copy(const CPUBuffer &src, const GPUBuffer &dst){
    SX_CORE_ASSERT(src.Size() == dst.Size(), "DMA::Copy, buffers should be the same size");
    Copy(src, dst, dst.Size());
}

sx_inline void DMA::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
    s_VTable.CopyCPU2GPUBuffer(src, dst, size, src_offset, dst_offset);
}

sx_inline void DMA::Copy(const CPUTexture &src, const GPUTexture &dst){
    s_VTable.CopyCPU2GPUTexture(src, dst);
}

sx_inline void DMA::ChangeLayout(GPUTexture &texture, GPUTexture::Layout layout){
    s_VTable.ChangeGPUTextureLayout(texture, layout);
}

}//namespace StraitX::

#endif//STRAITX_DMA_CONTEXT_HPP