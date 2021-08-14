#include <stdlib.h>
#include <unistd.h>
#include "platform/linux/opengl_context_impl.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( X11::Display *dpy, X11::XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

typedef X11::GLXContext (*glXCreateContextAttribsARBProc)(X11::Display*, X11::GLXFBConfig, X11::GLXContext, Bool, const int*);

Result OpenGLContextImpl::Create(const WindowImpl &window, const Version &version){
    m_WindowHandle = window.Handle;

    
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)X11::glXGetProcAddressARB((const X11::GLubyte *)"glXCreateContextAttribsARB");

    if(glXCreateContextAttribsARB == nullptr)
        return Result::Unsupported;

    int contextAttribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, version.Major,
		GLX_CONTEXT_MINOR_VERSION_ARB, version.Minor,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
    // OpenGL 3.0 requires context error handler
    if(version.Major >= 3)
        X11::XSetErrorHandler(&ctxErrorHandler);
    
    m_Handle = glXCreateContextAttribsARB(DisplayServer::Handle,(X11::GLXFBConfig)window.FBConfig, 0, true, contextAttribs);

    if(m_Handle == nullptr)
        return Result::Unsupported;

	if (!X11::glXIsDirect (DisplayServer::Handle, m_Handle)){
        Destroy();
        return Result::Failure;
    }

    return Result::Success;
}

Result OpenGLContextImpl::CreateLegacy(const WindowImpl &window){
    return Create(window, {4, 6, 0});
}

void OpenGLContextImpl::Destroy(){
    X11::glXDestroyContext(DisplayServer::Handle, m_Handle);
    m_WindowHandle = 0;
    m_Handle = nullptr;
}

void OpenGLContextImpl::DestroyLegacy(){
    Destroy();
}

Result OpenGLContextImpl::MakeCurrent(){
    return ResultError(X11::glXMakeCurrent(DisplayServer::Handle, m_WindowHandle, m_Handle) != True);
}

void OpenGLContextImpl::SwapBuffers(){
    X11::glXSwapBuffers(DisplayServer::Handle, m_WindowHandle);
}

void OpenGLContextImpl::Resize(u32 width, u32 height){
	(void)width;
	(void)height;
}


} // namespace Linux::
