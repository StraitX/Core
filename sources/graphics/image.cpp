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
    Clear();
}

Image &Image::operator=(Image &&other)noexcept{
    Clear();

    m_Data = other.m_Data;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    other.m_Data = nullptr;
    other.m_Width = 0;
    other.m_Height = 0;
    return *this;
}

void Image::Create(u32 width, u32 height, const Color &color){
    SX_CORE_ASSERT(IsEmpty(), "Image::Create: object is not empty");

    m_Width = width;
    m_Height = height;

    m_Data = (u32*)Memory::Alloc((sizeof(u32)) * m_Width * m_Height);

    Fill(color);
}

void Image::Clear(){
    Memory::Free(m_Data); 
    m_Data = nullptr;
    m_Width = 0;
    m_Height = 0;
}

Result Image::LoadFromFile(const char *filename){
    if(!File::Exists(filename))return Result::NotFound;

    File file;
    if(!file.Open(filename, File::Mode::Read))return Result::Failure;

    return LoadFromFile(file);
}

Result Image::LoadFromFile(File &file){
    SX_CORE_ASSERT(IsEmpty(), "Image::Load: object is not empty");
    
    u8 channels = 0;
    void *pointer = nullptr;

    if(!ImageLoader::LoadImage(file, m_Width, m_Height, 4, channels, pointer))return Result::Failure;
    m_Data = (u32*)pointer;

    if (channels != 4){
        Clear();
        return Result::WrongFormat;
    }

    return Result::Success;
}

Result Image::SaveToFile(File &file, ImageFileFormat save_format){
    return ImageLoader::SaveImage(file, m_Width, m_Height, 4, save_format, m_Data);
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
    u32 pixel = color.RGBA8();

    for(u32 i = 0; i<m_Height; i++){
        u32 offset = i * m_Width;
        for(u32 j = 0; j<m_Width; j++){
            pixels[offset + j] = pixel;
        }
    }
}
