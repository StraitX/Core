#ifndef STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP
#define STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{
namespace Vk{

struct GPUTextureImpl{
    static VkImageLayout s_LayoutTable[];
    static VkFormat s_FormatTable[];

    VkImage &Handle;
    VkImageView &ViewHandle;
    VkDeviceMemory &Memory;
    u32 &Width;
    u32 &Height;
    GPUTexture::Layout &Layout;
    TextureFormat &Format;
    GPUTexture::Usage  &Usage;

    GPUTextureImpl(GPUTexture &texture);

    void Create(TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    void Destroy();

    void CreateWithImage(VkImage image, GPUTexture::Layout layout, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    void DestroyWithoutImage();

    static VkSampleCountFlagBits ToVkSampleCount(SamplePoints samples);

    static void NewImpl(GPUTexture &texture, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    static void DeleteImpl(GPUTexture &texture);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP