#include "graphics/api/texture.hpp"

bool IsDepthFormat(TextureFormat format){
    if(format == TextureFormat::Depth32
    || format == TextureFormat::Depth24Stencil8)
        return true;
    return false;
}

Texture2D *Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureUsage usage, TextureLayout initial_layout){
    return nullptr;
}
