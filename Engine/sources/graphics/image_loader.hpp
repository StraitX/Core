#ifndef STRAITX_IMAGE_LOADER_HPP
#define STRAITX_IMAGE_LOADER_HPP

#include "platform/file.hpp"
#include "platform/result.hpp"
#include "graphics/api/format.hpp"

enum class ImageFormat{
    PNG = 1,
    JPG,
    BMP,
    TGA
};

struct ImageLoader{
    //Loads Image in RGBA8 format
    static Result LoadImage(File &file, u32 &width, u32 &height, PixelFormat format, u8 *&pixels);

    static Result SaveImage(File &file, u32 width, u32 height, PixelFormat p_format, ImageFormat i_format, u8 *pixels);
};

#endif//STRAITX_IMAGE_LOADER_HPP