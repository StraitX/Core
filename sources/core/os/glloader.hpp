#ifndef STRAITX_GLLOADER_HPP
#define STRAITX_GLLOADER_HPP 

#include "core/result.hpp"
#include "core/types.hpp"

class OpenGLLoader{
public:
    // we can load OpenGL only when current context exist
    static Result Load();
    
    static Version OpenGLVersion();

    static bool IsSupported(const char* extension_name);

    static bool LoadExtensions();

    //defined in platform dependent code
    static bool LoadPlatformExtensions();

    static void *GetOpenGLProc(const char *name);
};

#endif //STRAITX_GLLOADER_HPP 