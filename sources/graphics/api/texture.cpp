#include "graphics/api/texture.hpp"

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
    return nullptr;
}
