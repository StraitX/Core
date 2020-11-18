#include "platform/linux/glcontext_x11.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#undef Success

namespace StraitX{
namespace Linux{

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( ::Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

typedef GLXContext (*glXCreateContextAttribsARBProc)(::Display*, GLXFBConfig, GLXContext, Bool, const int*);

GLContextX11::GLContextX11(WindowX11 &window):
    m_Window(window)
{}

Error GLContextX11::Create(const FBConfigX11 &config, const Version &version){
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

    if(glXCreateContextAttribsARB == nullptr)
        return Error::NotSupported;

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
    
    m_Handle = glXCreateContextAttribsARB(m_Window.Display().Handle(),(GLXFBConfig)m_Window.FBConfig().Handle(), 0, true, contextAttribs);

    if(m_Handle == nullptr)
        return Error::NotSupported;

	if (!glXIsDirect (m_Window.Display().Handle(), m_Handle)){
        Destory();
        return Error::Failure;
    }

    return MakeCurrent();
}

void GLContextX11::Destory(){
    glXDestroyContext(m_Window.Display().Handle(), m_Handle);
}

Error GLContextX11::MakeCurrent(){
    if(glXMakeCurrent(m_Window.Display().Handle(), m_Window.Handle(), m_Handle))
        return Error::Success;
    return Error::Failure;
}

void GLContextX11::SwapBuffers(){
    glXSwapBuffers(m_Window.Display().Handle(), m_Window.Handle());
}

};
}; // namespace StraitX::