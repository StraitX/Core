#ifndef STRAITX_GLLOADER_HPP
#define STRAITX_GLLOADER_HPP 

#include "platform/error.hpp"
#include "platform/types.hpp"

namespace StraitX{

class OpenGLLoader{
public:
    // we can load OpenGL only when current context exist
    static Error Load();
    
    static Version OpenGLVersion();

    //defined in platform dependent code
    static void *GetOpenGLProc(const char *name);
};

}; // namespace StraitX::

#endif //STRAITX_GLLOADER_HPP 