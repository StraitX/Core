#ifndef STRAITX_IMAGE_LOADER_HPP
#define STRAITX_IMAGE_LOADER_HPP

#include "platform/file.hpp"
#include "platform/memory.hpp"

namespace StraitX{

struct ImageLoader{
    //Loads Image in RGBA8 format
    static bool LoadImage(const char *filename, u32 &width, u32 &height, u8 *& pixels);

    static bool LoadImage(File &file, u32 &width, u32 &height, u8 *& pixels);
};

sx_inline bool ImageLoader::LoadImage(const char *filename, u32 &width, u32 &height, u8 *& pixels){
    if(!File::Exist(filename))return false;

    File file;
    if(file.Open(filename, File::Mode::Read) == Result::Success){
        auto res = LoadImage(file, width, height, pixels);
        file.Close();
        return res;
    }
    return false;
}

}//namespace StraitX::

#endif//STRAITX_IMAGE_LOADER_HPP