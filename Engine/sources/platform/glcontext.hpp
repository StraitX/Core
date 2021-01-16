#ifndef STRAITX_GLCONTEXT_HPP
#define STRAITX_GLCONTEXT_HPP

#include "platform/platform_detection.hpp"
#include "platform/window.hpp"
#include "platform/noncopyable.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/glcontext_impl.hpp"
    typedef StraitX::Linux::GLContextImpl PlatformGLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

namespace StraitX{

class GLContext: public NonCopyable{
private:
    PlatformGLContextImpl m_Impl;
public:

    GLContext() = default;

    Result Create(Window &window, const Version &version);

    Result CreateDummy(const Version &version);

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();
};

sx_inline Result GLContext::Create(Window &window, const Version &version){
    return m_Impl.Create(window.Impl(), version);
}

sx_inline Result GLContext::CreateDummy(const Version &version){
    return m_Impl.CreateDummy(version);
}

sx_inline void GLContext::DestroyDummy(){
    m_Impl.DestroyDummy();
}

sx_inline void GLContext::Destroy(){
    m_Impl.Destroy();
}

sx_inline Result GLContext::MakeCurrent(){
    return m_Impl.MakeCurrent();
}

sx_inline void GLContext::SwapBuffers(){
    m_Impl.SwapBuffers();
}

}; // namespace StraitX::

#endif // STRAITX_GLCONTEXT_X11_HPP