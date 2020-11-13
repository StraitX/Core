#include "platform/fbconfig.hpp"


namespace StraitX{

FBConfig::FBConfig():
    m_Impl()
{}

Error FBConfig::PickDefault(){
    return m_Impl.PickDefault();
}

Error FBConfig::PickDesired(const PixelFormat &desired){
    return m_Impl.PickDesired(desired);
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