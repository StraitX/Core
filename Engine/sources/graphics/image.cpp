#include <cstdlib>
#include "graphics/image.hpp"
#include "graphics/image_loader.hpp"

namespace StraitX{

Image::~Image(){
    std::free(m_Data); 
}

bool Image::LoadFromFile(const char *filename, PixelFormat format){
    if(!File::Exist(filename))return false;

    File file;
    if(file.Open(filename, File::Mode::Read) == Result::Success){
        auto res = LoadFromFile(file, format);
        file.Close();
        return res;
    }
    return false;
}

bool Image::LoadFromFile(File &file, PixelFormat format){
    if(ImageLoader::LoadImage(file, m_Width, m_Height, format, m_Data)){
        m_Format = format;
        return true;
    }
    return false;
}

}//namespace StraitX::