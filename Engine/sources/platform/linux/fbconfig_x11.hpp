#ifndef STRAITX_FBCONFIG_X11_HPP
#define STRAITX_FBCONFIG_X11_HPP

#include "platform/types.hpp"

namespace StraitX{
namespace Linux{

class FBConfigX11{
private:
    PixelFormat m_PixelFormat;
    void *m_Handle;
public:
    FBConfigX11();

    FBConfigX11(const PixelFormat &desired);

    bool IsValid() const;

    const PixelFormat &Pixel() const;

    void *Handle();

};

}; //namespace Linux::
}; //namespace StraitX::


#endif // STRAITX_FBCONFIG_X11_HPP