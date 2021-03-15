#include "stb_image.h"
#include "graphics/image_loader.hpp"

namespace StraitX{

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

bool ImageLoader::LoadImage(File &file, u32 &width, u32 &height, u8 *&pixels){
    constexpr int desired_channels = 4;
    int x, y, channels;
    auto data = stbi_load_from_callbacks(&stb_callbacks, &file, &x, &y, &channels, desired_channels);

    width = 0;
    height = 0;
    pixels = nullptr;

    if(data == nullptr)return false;

    if(channels != desired_channels){
        stbi_image_free(data);
        return false;
    }

    width = x;
    height = y;
    pixels = data;

    return true;
}


}//namespace StraitX::