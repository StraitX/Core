#ifndef STRAITX_FBCONFIG_X11_HPP
#define STRAITX_FBCONFIG_X11_HPP

#include "platform/types.hpp"

namespace StraitX{
namespace Linux{

class FBConfigX11{
private:
    void *m_Handle;
public:
    FBConfigX11(const PixelFormat &desired, PixelFormat &resulted);


    void *Handle();

};

}; //namespace Linux::
}; //namespace StraitX::


#endif // STRAITX_FBCONFIG_X11_HPP