#include "platform/fbconfig.hpp"


namespace StraitX{

FBConfig::FBConfig():
    m_Impl()
{}

Error FBConfig::PickDefault(Display &display, const Screen &screen){
    return m_Impl.PickDefault(display.Impl(), screen.Impl());
}

Error FBConfig::PickDesired(Display &display, const Screen &screen,const PixelFormat &desired){
    return m_Impl.PickDesired(display.Impl(),screen.Impl(),desired);
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