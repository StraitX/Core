#ifndef STRAITX_GLCONTEXT_HPP
#define STRAITX_GLCONTEXT_HPP

#include "platform/platform_detection.hpp"
#include "platform/fbconfig.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/glcontext_x11.hpp"
    typedef StraitX::Linux::GLContextX11 GLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

namespace StraitX{

class GLContext{
private:
    GLContextImpl m_Impl;
public:
    GLContext(Window &window);

    Error Create(const FBConfig &config, const Version &version);

    void Destory();

    Error MakeCurrent();

    void SwapBuffers();
};

}; // namespace StraitX::

#endif // STRAITX_GLCONTEXT_X11_HPP