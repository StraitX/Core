#ifndef STRAITX_FBCONFIG_X11_HPP
#define STRAITX_FBCONFIG_X11_HPP

#include "platform/types.hpp"
#include "platform/error.hpp"
#include "platform/linux/display_x11.hpp"
#include "platform/linux/screen_x11.hpp"

namespace StraitX{
namespace Linux{

class FBConfigX11{
private:
    PixelFormat m_PixelFormat;
    DisplayX11 &m_Display;
    void *m_Handle;
    void *m_VisualInfo;
public:
    FBConfigX11(DisplayX11 &display);

    Error PickDefault(const ScreenX11 &screen);

    Error PickDesired(const PixelFormat &desired, const ScreenX11 &screen);

    const PixelFormat &Pixel() const;

    void *Handle() const;

    void *VisualInfo()const;

};

}; //namespace Linux::
}; //namespace StraitX::


#endif // STRAITX_FBCONFIG_X11_HPP