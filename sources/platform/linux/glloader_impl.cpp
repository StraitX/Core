#include "core/os/glloader.hpp"
#include "platform/linux/x11.hpp"

bool OpenGLLoader::LoadPlatformExtensions(){
    return true;
}

void *OpenGLLoader::GetOpenGLProc(const char *name){
    return (void*)X11::glXGetProcAddress((const X11::GLubyte *)name);
}