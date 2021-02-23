#include <Windows.h>
#include "platform/glloader.hpp"

namespace StraitX{

void *OpenGLLoader::GetOpenGLProc(const char *name){
    return reinterpret_cast<void*>(wglGetProcAddress((LPCSTR)name));
}

} // namespace StraitX::
