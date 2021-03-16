#ifndef STRAITX_FORMAT_HPP
#define STRAITX_FORMAT_HPP

#include "platform/types.hpp"

namespace StraitX{

enum class PixelFormat{
    RGBA8 = 1 
};

size_t GetPixelSize(PixelFormat format);

}//namespace StraitX::

#endif//STRAITX_FORMAT_HPP