#ifndef STRAITX_FBCONFIG_HPP
#define STRAITX_FBCONFIG_HPP

#include "platform/types.hpp"
#include "platform/platform_detection.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/fbconfig_x11.hpp"
    typedef StraitX::Linux::FBConfigX11 FBConfigImpl;
#else
    #error "Your platfrom does not support FrameBufferConfig yet"
#endif

namespace StraitX{

class FBConfig{
private:
    FBConfigImpl m_Impl;
public:
    FBConfig();

    FBConfig(const PixelFormat &desired);

    FBConfigImpl &Impl();

    bool IsValid();

    const PixelFormat &Pixel();
};

}; // namespace StraitX::

#endif // STRAITX_FBCONFIG_HPP