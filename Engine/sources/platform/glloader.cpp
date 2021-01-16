#include "platform/glloader.hpp"
#include <glad/glad.h>
#include <cstdio>
namespace StraitX{

Result OpenGLLoader::Load(){
    return ResultError(gladLoadGLLoader((GLADloadproc)GetOpenGLProc)==0);
}

Version OpenGLLoader::OpenGLVersion(){
    Version result = {0};
    const char *version = (const char *)glGetString(GL_VERSION);
    sscanf(version,"%hu.%hu.%u",&result.Major,&result.Minor, &result.Patch);
    return result;
}



}; // namespace StraitX::