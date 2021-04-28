#ifndef STRAITX_VULKAN_DMA_IMPL_HPP
#define STRAITX_VULKAN_DMA_IMPL_HPP

#include "graphics/api/dma.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/command_buffer.hpp"

namespace StraitX{
namespace Vk{

class DMAImpl{
private:
    Vk::CommandBuffer m_CmdBuffer{QueueFamily::Transfer};
    Vk::Fence m_OpFence;

    static u8 s_Instance[];
public:
    void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    void Copy(const CPUTexture &src, const GPUTexture &dst);

    void ChangeLayout(GPUTexture &texture, GPUTexture::Layout new_layout);

    static DMAImpl &Get(){
        return *reinterpret_cast<DMAImpl*>(s_Instance);
    }

    static void Initialize();

    static void Finalize();

    static void CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset);

    static void CopyCPU2GPUTextureImpl(const CPUTexture &src, const GPUTexture &dst);

    static void ChangeGPUTextureLayoutImpl(GPUTexture &src, GPUTexture::Layout layout);
};

}//namespace VK::
}//namespace StraitX::

#endif//STRAITX_VULKAN_DMA_IMPL_HPP