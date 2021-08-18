#ifndef STRAITX_IMAGE_LOADER_HPP
#define STRAITX_IMAGE_LOADER_HPP

#include "core/os/file.hpp"
#include "core/result.hpp"

enum class ImageFileFormat{
    PNG = 1,
    JPG,
    BMP,
    TGA
};

struct ImageLoader{
    //Loads Image in RGBA8 format
    static Result LoadImage(File &file, u32 &width, u32 &height, u8 desired_channels, u8 &loaded_channels, void *&pixels);

    static Result SaveImage(File &file, u32 width, u32 height, u8 channels, ImageFileFormat i_format, void *pixels);
};

#endif//STRAITX_IMAGE_LOADER_HPP