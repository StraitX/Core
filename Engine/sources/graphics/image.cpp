#include <cstdlib>
#include "platform/memory.hpp"
#include "core/string.hpp"
#include "core/move.hpp"
#include "graphics/image.hpp"
#include "graphics/image_loader.hpp"

namespace StraitX{

Image::Image(Image &&other){
    *this = Move(other);
}

Image::~Image(){
    std::free(m_Data); 
}

Image &Image::operator=(Image &&other){
    m_Data = other.m_Data;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Format = other.m_Format;
    other.m_Data = nullptr;
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_Format = {};
    return *this;
}

void Image::Create(u32 width, u32 height, const Color &color){
    m_Width = width;
    m_Height = height;
    m_Format = PixelFormat::RGBA8;

    m_Data = (u8*)std::malloc(GetPixelSize(m_Format) * m_Width * m_Height);


    u32 *pixels = (u32*)m_Data;
    u32 pixel = color.RGBA8();

    for(u32 i = 0; i<m_Height; i++){
        u32 offset = i * m_Width;
        for(u32 j = 0; j<m_Width; j++){
            pixels[offset + j] = pixel;
        }
    }
}

Result Image::LoadFromFile(const char *filename, PixelFormat format){
    if(!File::Exist(filename))return Result::NotFound;

    File file;
    if(file.Open(filename, File::Mode::Read)){
        auto res = LoadFromFile(file, format);
        file.Close();
        return res;
    }
    return Result::Failure;
}

Result Image::LoadFromFile(File &file, PixelFormat format){
    if(ImageLoader::LoadImage(file, m_Width, m_Height, format, m_Data)){
        m_Format = format;
        return Result::Success;
    }
    return Result::Failure;
}

Result Image::SaveToFile(File &file, ImageFormat save_format){
    return ImageLoader::SaveImage(file, m_Width, m_Height, m_Format, save_format, m_Data);
}

Result Image::SaveToFile(const char *filename){
    ImageFormat format;
    {
        const char *end = String::FindLast(filename, ".");

        if(!end)return Result::WrongFormat;

        auto length = String::Length(end);
        char *extension = (char*)alloca(length);

        Memory::Copy(end+1, ++extension, length);

        String::ToLowerCase(extension);

        if(String::Equals(extension, "png"))
            format = ImageFormat::PNG;
        else if(String::Equals(extension, "jpg") || String::Equals(extension, "jpeg"))
            format = ImageFormat::JPG;
        else if(String::Equals(extension, "tga"))
            format = ImageFormat::TGA;
        else if(String::Equals(extension, "bmp"))
            format = ImageFormat::BMP;
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

}//namespace StraitX::