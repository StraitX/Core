#include "platform/fbconfig.hpp"


namespace StraitX{

FBConfig::FBConfig():
    m_Impl()
{}

FBConfig::FBConfig(const PixelFormat &desired):
    m_Impl(desired)
{

}

FBConfigImpl &FBConfig::Impl(){
    return m_Impl;
}

bool FBConfig::IsValid(){
    return m_Impl.IsValid();
}

const PixelFormat &FBConfig::Pixel(){
    return m_Impl.Pixel();
}

}; // namespace StraitX::