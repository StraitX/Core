#ifndef STRAITX_OPENGL_CONTEXT_HPP
#define STRAITX_OPENGL_CONTEXT_HPP

#include "core/env/os.hpp"
#include "core/os/window.hpp"
#include "core/noncopyable.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/opengl_context_impl.hpp"
    typedef Linux::OpenGLContextImpl PlatformGLContextImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "core/os/windows/opengl_context_impl.hpp"
    typedef Windows::OpenGLContextImpl PlatformGLContextImpl;
#elif defined(SX_PLATFORM_MACOS)
    #include "platform/macos/opengl_context_impl.hpp"
    typedef MacOS::OpenGLContextImpl PlatformGLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

class OpenGLContext: public NonCopyable{
private:
    PlatformGLContextImpl m_Impl;
public:

    OpenGLContext() = default;

    Result Create(const PlatformWindowImpl &window, const Version &version);

    Result CreateLegacy(const PlatformWindowImpl &window);

    void Destroy();

    void DestroyLegacy();

    Result MakeCurrent();

    void SwapBuffers();
    // by spec OpenGL framebuffer should resize automaticaly, but MacOS does not seem to care
    void Resize(u32 width, u32 height);
};

SX_INLINE Result OpenGLContext::Create(const PlatformWindowImpl &window, const Version &version){
    return m_Impl.Create(window, version);
}

SX_INLINE Result OpenGLContext::CreateLegacy(const PlatformWindowImpl &window){
    return m_Impl.CreateLegacy(window);
}

SX_INLINE void OpenGLContext::DestroyLegacy(){
    m_Impl.DestroyLegacy();
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

SX_INLINE void OpenGLContext::Resize(u32 width, u32 height){
    m_Impl.Resize(width, height);
}

#endif // STRAITX_OPENGL_CONTEXT_HPP