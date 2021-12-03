#include "graphics/api/texture.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"
#include "core/assert.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/texture_impl.hpp"
#endif

bool IsDepthFormat(TextureFormat format){
    if(format == TextureFormat::Depth32
    || format == TextureFormat::Depth24Stencil8)
        return true;
    return false;
}

bool IsImageFormat(TextureFormat format){
    return !IsDepthFormat(format) && format != TextureFormat::Unknown;
}

size_t GetPixelSize(TextureFormat format){
    static const size_t s_FormatPixelSizeTable[]={
        0,
        4,
        3,
        4,
        4,
        4
    };

    return s_FormatPixelSizeTable[(size_t)format];
}

Texture2D *Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureUsage usage, TextureLayout initial_layout){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::Texture2DImpl(width, height, format, usage, initial_layout);
#endif
    return nullptr;
}

Texture2D *Texture2D::Create(const Image &image, TextureUsage usage, TextureLayout initial_layout){
    //SX_CORE_ASSERT(image.Channels() == 3, "Only RGBA images are supported now");

    usage |= TextureUsageBits::TransferDst;

    Texture2D *texture = Create(image.Width(), image.Height(), TextureFormat::RGBA8, usage, initial_layout);

    texture->Copy(image.Data(), Vector2u(image.Width(), image.Height()));

    return texture;
}

Texture2D *Texture2D::Create(const char *filepath, TextureUsage usage, TextureLayout initial_layout){
    Image staging;
    Result res = staging.LoadFromFile(filepath);

    SX_CORE_ASSERT(res == Result::Success, "Can't load image from a file");

    return Create(staging, usage, initial_layout);
}
