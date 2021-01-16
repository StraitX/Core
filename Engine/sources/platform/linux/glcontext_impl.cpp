#include "platform/linux/glcontext_impl.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#undef Success

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
    m_Window = &window;

    
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
    
    m_Handle = glXCreateContextAttribsARB(s_Display,(GLXFBConfig)m_Window->FBConfig, 0, true, contextAttribs);

    if(m_Handle == nullptr)
        return Result::Unsupported;

	if (!glXIsDirect (s_Display, m_Handle)){
        Destory();
        return Result::Failure;
    }

    return Result::Success;
}

void GLContextImpl::Destory(){
    glXDestroyContext(s_Display, m_Handle);
}

Result GLContextImpl::MakeCurrent(){
    return ResultError(glXMakeCurrent(s_Display, m_Window->Handle, m_Handle) == 0);
}

void GLContextImpl::SwapBuffers(){
    glXSwapBuffers(s_Display, m_Window->Handle);
}

} // namespace Linux::
} // namespace StraitX::