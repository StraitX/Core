#include "platform/glloader.hpp"
#include <GL/glx.h>
#include <GL/gl.h>
#undef Success

bool OpenGLLoader::LoadPlatformExtensions(){
    return true;
}

void *OpenGLLoader::GetOpenGLProc(const char *name){
    return (void*)glXGetProcAddress((const GLubyte *)name);
}