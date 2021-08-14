#ifndef STRAITX_OPENGL_CONTEXT_HPP
#define STRAITX_OPENGL_CONTEXT_HPP

#include "core/env/os.hpp"
#include "core/os/window.hpp"
#include "core/noncopyable.hpp"

#ifdef SX_OS_LINUX
    #include "platform/linux/opengl_context_impl.hpp"
    typedef Linux::OpenGLContextImpl OSOpenGLContextImpl;
#elif defined(SX_OS_WINDOWS)
    #include "platform/windows/opengl_context_impl.hpp"
    typedef Windows::OpenGLContextImpl OSOpenGLContextImpl;
#elif defined(SX_OS_MACOS)
    #include "platform/macos/opengl_context_impl.hpp"
    typedef MacOS::OpenGLContextImpl OSOpenGLContextImpl;
#else
    #error "Your platform does not support OpenGL context"
#endif

class OpenGLContext: private OSOpenGLContextImpl, public NonCopyable{
private:
    using Super = OSOpenGLContextImpl;
public:
    OpenGLContext() = default;

    Result Create(const Window &window, const Version &version);

    Result CreateLegacy(const Window &window);

    void Destroy();

    void DestroyLegacy();

    Result MakeCurrent();

    void SwapBuffers();
    // by spec OpenGL framebuffer should resize automaticaly, but MacOS does not seem to care
    void Resize(u32 width, u32 height);
};

SX_INLINE Result OpenGLContext::Create(const Window &window, const Version &version){
    return Super::Create(window.Impl(), version);
}

SX_INLINE Result OpenGLContext::CreateLegacy(const Window &window){
    return Super::CreateLegacy(window.Impl());
}

SX_INLINE void OpenGLContext::DestroyLegacy(){
    Super::DestroyLegacy();
}

SX_INLINE void OpenGLContext::Destroy(){
    Super::Destroy();
}

SX_INLINE Result OpenGLContext::MakeCurrent(){
    return Super::MakeCurrent();
}

SX_INLINE void OpenGLContext::SwapBuffers(){
    Super::SwapBuffers();
}

SX_INLINE void OpenGLContext::Resize(u32 width, u32 height){
    Super::Resize(width, height);
}

#endif // STRAITX_OPENGL_CONTEXT_HPP