#ifndef STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP
#define STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP

#include "graphics/api/gpu_texture.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GPUTextureImpl{
    static VkImageLayout s_LayoutTable[];
    static VkFormat s_FormatTable[];

    Vk::LogicalGPUImpl *const Owner;
    VkImage &Handle;
    VkImageView &ViewHandle;
    VkDeviceMemory &Memory;
    u32 &Width;
    u32 &Height;
    GPUTexture::Layout &Layout;
    GPUTexture::Format &Format;
    GPUTexture::Usage  &Usage;

    sx_inline GPUTextureImpl(GPUTexture &texture);

    void Create(GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height);

    void CreateFromImage(VkImage image, GPUTexture::Format format, u32 width, u32 height);

    void Destroy();

    static VkImageView CreateImageView(const Vk::LogicalGPUImpl *owner, VkImage image, GPUTexture::Format format, u32 width, u32 height);

    static void DestroyImageView(const Vk::LogicalGPUImpl *owner, VkImageView view);

    static VkSampleCountFlagBits ToVkSampleCount(SamplePoints samples);

    static void NewImpl(GPUTexture &texture, GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height);

    static void DeleteImpl(GPUTexture &texture);

};

sx_inline GPUTextureImpl::GPUTextureImpl(GPUTexture &texture):
    Owner(static_cast<Vk::LogicalGPUImpl *>(texture.m_Owner)),
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