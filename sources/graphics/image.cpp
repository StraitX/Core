#include <cstdlib>
#include "core/os/memory.hpp"
#include "core/string.hpp"
#include "core/move.hpp"
#include "graphics/image.hpp"
#include "graphics/image_loader.hpp"

Image::Image(u32 width, u32 height, const Color &color){
    Create(width, height, color);
}

Image::Image(Image &&other)noexcept{
    *this = Move(other);
}

Image::~Image(){
    Free();
}

Image &Image::operator=(Image &&other)noexcept{
    Free();

    m_Data = other.m_Data;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Channels = other.m_Channels;
    other.m_Data = nullptr;
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_Channels = {};
    return *this;
}

void Image::Create(u32 width, u32 height, const Color &color){
    SX_CORE_ASSERT(IsEmpty(), "Image::Create: object is not empty");

    m_Width = width;
    m_Height = height;
    m_Channels = 4;

    m_Data = (u8*)std::malloc((sizeof(byte) * m_Channels) * m_Width * m_Height);

    Fill(color);
}

void Image::Free(){
    std::free(m_Data); 
    m_Data = nullptr;
    m_Width = 0;
    m_Height = 0;
    m_Channels = {};
}

Result Image::LoadFromFile(const char *filename, u8 desired_channels){
    if(!File::Exist(filename))return Result::NotFound;

    File file;
    if(!file.Open(filename, File::Mode::Read))return Result::Failure;

    return LoadFromFile(file, desired_channels);
}

Result Image::LoadFromFile(File &file, u8 desired_channels){
    SX_CORE_ASSERT(IsEmpty(), "Image::Load: object is not empty");

    if(!ImageLoader::LoadImage(file, m_Width, m_Height, desired_channels, m_Channels, m_Data))return Result::Failure;

    return Result::Success;
}

Result Image::SaveToFile(File &file, ImageFileFormat save_format){
    return ImageLoader::SaveImage(file, m_Width, m_Height, m_Channels, save_format, m_Data);
}

Result Image::SaveToFile(const char *filename){
    ImageFileFormat format;
    {
        const char *end = String::FindLast(filename, ".");

        if(!end)return Result::WrongFormat;

        auto length = String::Length(end) + 1;
        char *extension = (char*)alloca(length);

        Memory::Copy(end+1, extension, length);

        String::ToLowerCase(extension);

        if(String::Equals(extension, "png"))
            format = ImageFileFormat::PNG;
        else if(String::Equals(extension, "jpg") || String::Equals(extension, "jpeg"))
            format = ImageFileFormat::JPG;
        else if(String::Equals(extension, "tga"))
            format = ImageFileFormat::TGA;
        else if(String::Equals(extension, "bmp"))
            format = ImageFileFormat::BMP;
        else return Result::WrongFormat;
    }

    File file;
    if(file.Open(filename, File::Mode::Write)){
        auto res = SaveToFile(file, format);
        file.Close();
        return res;
    }
    return Result::Failure;
}

void Image::Fill(const Color &color){
    
    u32 *pixels = (u32*)m_Data;
    // we use here ABGR because of little-endian cpu assumption 
    u32 pixel = color.ABGR8();

    for(u32 i = 0; i<m_Height; i++){
        u32 offset = i * m_Width;
        for(u32 j = 0; j<m_Width; j++){
            pixels[offset + j] = pixel;
        }
    }
}
