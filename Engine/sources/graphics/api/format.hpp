#ifndef STRAITX_FORMAT_HPP
#define STRAITX_FORMAT_HPP

#include "platform/types.hpp"

namespace StraitX{

enum class PixelFormat: u8{
    RGBA8 = 1 
};

enum class TextureFormat : u8{
    Unknown = 0,
    RGBA8,
    Depth24Stencil8,
    BGRA8
};

size_t GetPixelSize(PixelFormat format);

size_t GetPixelSize(TextureFormat format);


bool IsDepthFormat(TextureFormat format);

bool IsColorFormat(TextureFormat format);

TextureFormat ToTextureFormat(PixelFormat format);

}//namespace StraitX::

#endif//STRAITX_FORMAT_HPP