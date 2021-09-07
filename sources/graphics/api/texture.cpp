#include "graphics/api/texture.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

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
