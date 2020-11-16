#include "platform/fbconfig.hpp"


namespace StraitX{

FBConfig::FBConfig(StraitX::Display &display):
    m_Impl(display.Impl())
{}

Error FBConfig::PickDefault(const Screen &screen){
    return m_Impl.PickDefault(screen.Impl());
}

Error FBConfig::PickDesired(const PixelFormat &desired, const Screen &screen){
    return m_Impl.PickDesired(desired,screen.Impl());
}

FBConfigImpl &FBConfig::Impl(){
    return m_Impl;
}

const FBConfigImpl &FBConfig::Impl()const{
    return m_Impl;
}


const PixelFormat &FBConfig::Pixel()const{
    return m_Impl.Pixel();
}

}; // namespace StraitX::