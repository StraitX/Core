#include "platform/linux/glcontext_impl.hpp"
#include <X11/Xlib.h>
#include <stdlib.h>
#include <GL/glx.h>
#undef Success
#include <unistd.h>
namespace StraitX{
namespace Linux{

extern ::Display *s_Display;    

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( ::Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

typedef GLXContext (*glXCreateContextAttribsARBProc)(::Display*, GLXFBConfig, GLXContext, Bool, const int*);

Result GLContextImpl::Create(WindowImpl &window, const Version &version){
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

Result GLContextImpl::CreateDummy(const Version &version){
    WindowImpl window;
    window.FBConfig = WindowImpl::PickBestFBConfig(DefaultScreen(s_Display));

    if(window.FBConfig == nullptr)
        return Result::Unsupported;

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(s_Display, (GLXFBConfig)window.FBConfig);

    if(visualInfo == nullptr)
        return Result::Failure;
    
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(s_Display,DefaultScreen(s_Display));
    attributes.colormap = XCreateColormap(s_Display, RootWindow(s_Display,DefaultScreen(s_Display)), visualInfo->visual, AllocNone);

    window.Handle = XCreateWindow(s_Display, RootWindow(s_Display,DefaultScreen(s_Display)), 0, 0, 1, 1, 0, visualInfo->depth,
        InputOutput, visualInfo->visual, CWBackPixel | CWColormap, &attributes);

    return Create(window, version);
}

void GLContextImpl::Destroy(){
    glXDestroyContext(s_Display, m_Handle);
    m_WindowHandle = 0;
    m_Handle = nullptr;
}

void GLContextImpl::DestroyDummy(){
    XDestroyWindow(s_Display, m_WindowHandle);
    Destroy();
}

Result GLContextImpl::MakeCurrent(){
    return ResultError(glXMakeCurrent(s_Display, m_WindowHandle, m_Handle) != True);
}

void GLContextImpl::SwapBuffers(){
    glXSwapBuffers(s_Display, m_WindowHandle);
}

} // namespace Linux::
} // namespace StraitX::