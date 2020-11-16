#include "platform/linux/screen_x11.hpp"

namespace StraitX{
namespace Linux{

ScreenX11::ScreenX11(unsigned int index, void *handle, const Size2i &size, const Size2f &dpi):
    m_Index(index),
    m_Handle(handle),
    m_Size(size),
    m_DPI(dpi)
{}
    
const Size2i &ScreenX11::Size()const{
    return m_Size;
}

const Size2f &ScreenX11::DPI()const{
    return m_DPI;
}

}; // namespace Linux::
}; // namespace StraitX::