#include "stb_image.h"
#include "graphics/image_loader.hpp"

static int stb_read(void *user,char *data,int size){
    File *file = static_cast<File*>(user);
    return file->Read(data, size);
}
static void stb_skip(void *user,int n){
    File *file = static_cast<File*>(user);
    file->Seek(File::Current, n);
}
static int stb_eof(void *user){
    File *file = static_cast<File*>(user);
    return file->Seek(File::Current, 0) >= file->Seek(File::End, 0);
}

stbi_io_callbacks stb_callbacks = {
    stb_read,
    stb_skip,
    stb_eof
};

Result ImageLoader::LoadImage(File &file, u32 &width, u32 &height, u8 desired_channels, u8 &loaded_channels, void *&pixels){
    int x, y, channels;
    auto data = stbi_load_from_callbacks(&stb_callbacks, &file, &x, &y, &channels, desired_channels);

    width = 0;
    height = 0;
    pixels = nullptr;

    if(data == nullptr)return Result::Failure;

    width = x;
    height = y;
    pixels = data;
    loaded_channels = (u8)channels;

    return Result::Success;
}

static void stbi_write_function(void *context, void *data, int size){
    File *file = static_cast<File*>(context);
    file->Write(data, size);
}

Result ImageLoader::SaveImage(File &file, u32 width, u32 height, u8 channels, ImageFileFormat i_format, void *pixels){
    switch (i_format) {
    case ImageFileFormat::PNG:
        return Result((bool)stbi_write_png_to_func(stbi_write_function, &file, width, height, channels, pixels, width * channels));
    case ImageFileFormat::JPG:
        return Result((bool)stbi_write_jpg_to_func(stbi_write_function, &file, width, height, channels, pixels, 100));
    case ImageFileFormat::BMP:
        return Result((bool)stbi_write_bmp_to_func(stbi_write_function, &file, width, height, channels, pixels));
    case ImageFileFormat::TGA:
        return Result((bool)stbi_write_tga_to_func(stbi_write_function, &file, width, height, channels, pixels));
    }
    return Result::Success;
}
