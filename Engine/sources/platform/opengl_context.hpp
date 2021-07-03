#ifndef STRAITX_OPENGL_CONTEXT_HPP
#define STRAITX_OPENGL_CONTEXT_HPP

#include "platform/platform_detection.hpp"
#include "platform/window.hpp"
#include "platform/noncopyable.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/opengl_context_impl.hpp"
    typedef StraitX::Linux::OpenGLContextImpl PlatformGLContextImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/opengl_context_impl.hpp"
    typedef StraitX::Windows::OpenGLContextImpl PlatformGLContextImpl;
#elif defined(SX_PLATFORM_MACOS)
    #include "platform/macos/opengl_context_impl.hpp"
    typedef StraitX::MacOS::OpenGLContextImpl PlatformGLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

namespace StraitX{

class OpenGLContext: public NonCopyable{
private:
    PlatformGLContextImpl m_Impl;
public:

    OpenGLContext() = default;

    Result Create(const PlatformWindow &window, const Version &version);

    Result CreateDummy();

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();
};

SX_INLINE Result OpenGLContext::Create(const PlatformWindow &window, const Version &version){
    return m_Impl.Create(window.Impl(), version);
}

SX_INLINE Result OpenGLContext::CreateDummy(){
    return m_Impl.CreateDummy();
}

SX_INLINE void OpenGLContext::DestroyDummy(){
    m_Impl.DestroyDummy();
}

SX_INLINE void OpenGLContext::Destroy(){
    m_Impl.Destroy();
}

SX_INLINE Result OpenGLContext::MakeCurrent(){
    return m_Impl.MakeCurrent();
}

SX_INLINE void OpenGLContext::SwapBuffers(){
    m_Impl.SwapBuffers();
}

}// namespace StraitX::

#endif // STRAITX_OPENGL_CONTEXT_HPP