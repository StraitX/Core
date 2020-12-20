#ifndef STRAITX_GLCONTEXT_HPP
#define STRAITX_GLCONTEXT_HPP

#include "platform/platform_detection.hpp"
#include "platform/window.hpp"
#include "platform/noncopyable.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/glcontext_x11.hpp"
    typedef StraitX::Linux::GLContextX11 GLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

namespace StraitX{

class GLContext: public NonCopyable{
private:
    GLContextImpl m_Impl;
public:

    sx_inline GLContext(Window &window):
        m_Impl(window.Impl())
    {}

    sx_inline Result Create(const Version &version){
        return m_Impl.Create(version);
    }

    sx_inline void Destory(){
        m_Impl.Destory();
    }

    sx_inline Result MakeCurrent(){
        return m_Impl.MakeCurrent();
    }

    sx_inline void SwapBuffers(){
        m_Impl.SwapBuffers();
    }
};

}; // namespace StraitX::

#endif // STRAITX_GLCONTEXT_X11_HPP