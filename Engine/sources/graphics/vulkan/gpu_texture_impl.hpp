#ifndef STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP
#define STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP

#include "graphics/api/gpu_texture.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GPUTextureImpl{
    static VkImageLayout s_LayoutTable[];
    static VkFormat s_FormatTable[];

    LogicalGPU *&Owner;
    VkImage &Handle;
    VkImageView &ViewHandle;
    VkDeviceMemory &Memory;
    u32 &Width;
    u32 &Height;
    GPUTexture::Layout &Layout;
    TextureFormat &Format;
    GPUTexture::Usage  &Usage;

    sx_inline GPUTextureImpl(GPUTexture &texture);

    void Create(LogicalGPU &owner, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    void Destroy();

    void CreateWithImage(Vk::LogicalGPUImpl *owner, VkImage image, GPUTexture::Layout layout, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    void DestroyWithoutImage();

    static VkSampleCountFlagBits ToVkSampleCount(SamplePoints samples);

    static void NewImpl(GPUTexture &texture, LogicalGPU &owner, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    static void DeleteImpl(GPUTexture &texture);

};

sx_inline GPUTextureImpl::GPUTextureImpl(GPUTexture &texture):
    Owner(texture.m_Owner),
    Handle(reinterpret_cast<VkImage&>(texture.m_Handle.U64)),
    ViewHandle(reinterpret_cast<VkImageView&>(texture.m_ViewHandle.U64)),
    Memory(reinterpret_cast<VkDeviceMemory&>(texture.m_BackingMemory)),
    Width(texture.m_Width),
    Height(texture.m_Height),
    Layout(texture.m_Layout),
    Format(texture.m_Format),
    Usage(texture.m_Usage)
{}

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP