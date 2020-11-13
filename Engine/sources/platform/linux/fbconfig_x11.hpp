#ifndef STRAITX_FBCONFIG_X11_HPP
#define STRAITX_FBCONFIG_X11_HPP

#include "platform/types.hpp"
#include "platform/error.hpp"

namespace StraitX{
namespace Linux{

class FBConfigX11{
private:
    PixelFormat m_PixelFormat;
    void *m_Handle;
    void *m_VisualInfo;
public:
    FBConfigX11();

    Error PickDefault();

    Error PickDesired(const PixelFormat &desired);

    const PixelFormat &Pixel() const;

    void *Handle() const;

    void *VisualInfo()const;

};

}; //namespace Linux::
}; //namespace StraitX::


#endif // STRAITX_FBCONFIG_X11_HPP