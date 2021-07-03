#ifndef STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP
#define STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/texture.hpp"

namespace Vk{

struct TextureImpl{
    static VkImageLayout s_LayoutTable[];
    static VkFormat s_FormatTable[];

	static VkSampleCountFlagBits ToVkSampleCount(SamplePoints samples);
};

struct Texture2DImpl{
    VkImage &Handle;
    VkImageView &ViewHandle;
    VkDeviceMemory &Memory;
    u32 &Width;
    u32 &Height;
    TextureLayout &Layout;
    TextureFormat &Format;
    TextureUsageBits  &Usage;

    Texture2DImpl(Texture2D &texture);

    void Create(u32 width, u32 height, TextureFormat format, TextureUsageBits usage);

    void Destroy();

    void CreateWithImage(VkImage image, u32 width, u32 height, TextureLayout layout, TextureFormat format, TextureUsageBits usage);

    void DestroyWithoutImage();

    static void NewImpl(Texture2D &texture, u32 width, u32 height, TextureFormat format, TextureUsageBits usage);

    static void DeleteImpl(Texture2D &texture);

};

}//namespace Vk::

#endif//STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP