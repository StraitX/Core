#ifndef STRAITX_DMA_HPP
#define STRAITX_DMA_HPP

#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/cpu_texture.hpp"
#include "graphics/api/texture.hpp"

namespace StraitX{

class GraphicsAPILoader;

class DMA{
public:
    struct VTable{
        using CopyMem2GPUBufferProc = void (*)(const void *, const GPUBuffer &dst, u32 size, u32 dst_offset);
        using CopyCPU2GPUBufferProc = void (*)(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);
        using CopyCPU2GPUTextureProc = void (*)(const CPUTexture &src, const Texture2D &dst);
        using ChangeGPUTextureLayoutProc = void (*)(Texture &texture, TextureLayout layout);

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

    static void Copy(const CPUTexture &src, const Texture2D &dst);

    static void ChangeLayout(Texture &texture, TextureLayout layout);
};

SX_INLINE void DMA::Copy(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    s_VTable.CopyMem2GPUBuffer(src, dst, size, dst_offset);
}

SX_INLINE void DMA::Copy(const void *src, const GPUBuffer &dst){
    Copy(src, dst, dst.Size(), 0);
}

SX_INLINE void DMA::Copy(const CPUBuffer &src, const GPUBuffer &dst){
    SX_CORE_ASSERT(src.Size() == dst.Size(), "DMA::Copy, buffers should be the same size");
    Copy(src, dst, dst.Size());
}

SX_INLINE void DMA::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
    s_VTable.CopyCPU2GPUBuffer(src, dst, size, src_offset, dst_offset);
}

SX_INLINE void DMA::Copy(const CPUTexture &src, const Texture2D &dst){
    s_VTable.CopyCPU2GPUTexture(src, dst);
}

SX_INLINE void DMA::ChangeLayout(Texture &texture, TextureLayout layout){
    s_VTable.ChangeGPUTextureLayout(texture, layout);
}

}//namespace StraitX::

#endif//STRAITX_DMA_CONTEXT_HPP