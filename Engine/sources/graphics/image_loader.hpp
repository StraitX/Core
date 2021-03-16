#ifndef STRAITX_IMAGE_LOADER_HPP
#define STRAITX_IMAGE_LOADER_HPP

#include "platform/file.hpp"
#include "platform/memory.hpp"
#include "graphics/api/format.hpp"

namespace StraitX{

struct ImageLoader{
    //Loads Image in RGBA8 format
    static bool LoadImage(File &file, u32 &width, u32 &height, PixelFormat format, u8 *& pixels);
};

}//namespace StraitX::

#endif//STRAITX_IMAGE_LOADER_HPP