#include "platform/glloader.hpp"
#include <GL/glx.h>
#include <GL/gl.h>
#undef Success

namespace StraitX{

void *OpenGLLoader::GetOpenGLProc(const char *name){
    return (void*)glXGetProcAddress((const GLubyte *)name);
}

} // namespace StraitX::