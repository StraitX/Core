#include "platform/fbconfig.hpp"


namespace StraitX{

FBConfig::FBConfig(const PixelFormat &desired):
    m_Impl(desired,m_PixelFormat)
{

}

FBConfigImpl FBConfig::Impl(){
    return m_Impl;
}

const PixelFormat &FBConfig::Pixel(){
    return m_PixelFormat;
}

}; // namespace StraitX::