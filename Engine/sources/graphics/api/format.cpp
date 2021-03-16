#include "graphics/api/format.hpp"

namespace StraitX{

static size_t PixelFormatTable[] = {
    0,
    4
};

size_t GetPixelSize(PixelFormat format){
    return PixelFormatTable[(size_t)format];
}

}//namespace StraitX::