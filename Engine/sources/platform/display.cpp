#include "platform/display.hpp"

namespace StraitX{

Error Display::Open(){
    return m_Impl.Open();
}

Error Display::Close(){
    return m_Impl.Close();
}

bool Display::IsOpen(){
    return m_Impl.IsOpened();
}

bool Display::CheckSupport(Ext extension){
    return m_Impl.CheckSupport((DisplayImpl::Ext)extension);
}

Screen Display::MainScreen(){
    return Screen(m_Impl.MainScreen());
}

DisplayImpl &Display::Impl(){
    return m_Impl;
}

const DisplayImpl &Display::Impl()const{
    return m_Impl;
}

}; // namespace StraitX::