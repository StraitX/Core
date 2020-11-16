#ifndef STRAITX_SCREEN_X11_HPP
#define STRAITX_SCREEN_X11_HPP

#include "platform/types.hpp"

namespace StraitX{
namespace Linux{

class WindowX11;

class ScreenX11{
private:
    unsigned int m_Index;
    void *m_Handle;
    Size2i m_Size;
    Size2f m_DPI;

    friend class WindowX11;
public:
    ScreenX11(unsigned int index, void *handle, const Size2i &size, const Size2f &dpi);

    ~ScreenX11() = default;
    
    const Size2i &Size()const;

    const Size2f &DPI()const;
};

}; // namespace Linux::
}; // namespace StraitX::

#endif // STRAITX_DISPLAY_HPP