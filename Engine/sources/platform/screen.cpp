#include "platform/screen.hpp"

namespace StraitX{

Screen::Screen(const ScreenImpl &impl):
    m_Impl(impl)
{

}

const Size2i &Screen::Size()const{
    return m_Impl.Size();
}

const Size2f &Screen::DPI()const{
    return m_Impl.DPI();
}

ScreenImpl &Screen::Impl(){
    return m_Impl;
}

const ScreenImpl &Screen::Impl()const{
    return m_Impl;
}

}; // namespace StraitX::