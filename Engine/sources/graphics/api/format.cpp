#include "graphics/api/format.hpp"

namespace StraitX{

static size_t PixelFormatSizeTable[] = {
    0,
    4
};

size_t GetPixelSize(PixelFormat format){
    return PixelFormatSizeTable[(size_t)format];
}

static size_t TextureFormatSizeTable[]={
    0,
    4,
    4,
    4
};

size_t GetPixelSize(TextureFormat format){
    return TextureFormatSizeTable[(size_t)format];
}

bool IsDepthFormat(TextureFormat format){
    if(format == TextureFormat::Depth24Stencil8 || format == TextureFormat::Depth32)
        return true;
    return false;
}

bool IsColorFormat(TextureFormat format){
    if(format == TextureFormat::RGBA8 || format == TextureFormat::BGRA8)
        return true;
    return false;
}

TextureFormat ToTextureFormat(PixelFormat format){
    return TextureFormat::RGBA8;
}

}//namespace StraitX::