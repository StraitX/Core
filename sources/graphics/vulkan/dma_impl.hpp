#ifndef STRAITX_VULKAN_DMA_IMPL_HPP
#define STRAITX_VULKAN_DMA_IMPL_HPP

#include "graphics/dma.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/command_buffer.hpp"

namespace Vk{

class DMAImpl{
private:
    Vk::CommandBuffer m_CmdBuffer{QueueFamily::Transfer};
    Vk::Fence m_OpFence;

    static u8 s_Instance[];
private:
    void MemoryMapCopy(const void *src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    void CmdCopy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);
public:
    void Copy(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset);

    void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    void Copy(const CPUTexture &src, const Texture2D &dst);

    void ChangeLayout(Texture &texture, TextureLayout new_layout);

    static DMAImpl &Get(){
        return *reinterpret_cast<DMAImpl*>(s_Instance);
    }

    static void Initialize();

    static void Finalize();

    static void CopyMem2GPUBufferImpl(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset);

    static void CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    static void CopyCPU2GPUTextureImpl(const CPUTexture &src, const Texture2D &dst);

    static void ChangeGPUTextureLayoutImpl(Texture &src, TextureLayout layout);
};

}//namespace VK::

#endif//STRAITX_VULKAN_DMA_IMPL_HPP