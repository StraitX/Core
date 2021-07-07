#include "platform/linux/opengl_context_impl.hpp"
#include <X11/Xlib.h>
#include <stdlib.h>
#include <GL/glx.h>
#undef Success
#include <unistd.h>

namespace Linux{

extern ::Display *s_Display;    

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( ::Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

typedef GLXContext (*glXCreateContextAttribsARBProc)(::Display*, GLXFBConfig, GLXContext, Bool, const int*);

Result OpenGLContextImpl::Create(const WindowImpl &window, const Version &version){
    m_WindowHandle = window.Handle;

    
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

    if(glXCreateContextAttribsARB == nullptr)
        return Result::Unsupported;

    int contextAttribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, version.Major,
		GLX_CONTEXT_MINOR_VERSION_ARB, version.Minor,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};
    // OpenGL 3.0 requires context error handler
    if(version.Major >= 3)
        XSetErrorHandler(&ctxErrorHandler);
    
    m_Handle = glXCreateContextAttribsARB(s_Display,(GLXFBConfig)window.FBConfig, 0, true, contextAttribs);

    if(m_Handle == nullptr)
        return Result::Unsupported;

	if (!glXIsDirect (s_Display, m_Handle)){
        Destroy();
        return Result::Failure;
    }

    return Result::Success;
}

Result OpenGLContextImpl::CreateDummy(const WindowImpl &window){
    return Create(window, {4, 6, 0});
}

void OpenGLContextImpl::Destroy(){
    glXDestroyContext(s_Display, m_Handle);
    m_WindowHandle = 0;
    m_Handle = nullptr;
}

void OpenGLContextImpl::DestroyDummy(){
    Destroy();
}

Result OpenGLContextImpl::MakeCurrent(){
    return ResultError(glXMakeCurrent(s_Display, m_WindowHandle, m_Handle) != True);
}

void OpenGLContextImpl::SwapBuffers(){
    glXSwapBuffers(s_Display, m_WindowHandle);
}

void OpenGLContextImpl::Resize(u32 width, u32 height){
	(void)width;
	(void)height;
}


} // namespace Linux::
