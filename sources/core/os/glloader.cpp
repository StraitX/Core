#include "core/os/glloader.hpp"
#include <glad/glad.h>
#include <cstdio>
#include <string.h>



Result OpenGLLoader::Load(){
    //this should be called with a valid dummy context
    return ResultError(
        !gladLoadGLLoader((GLADloadproc)GetOpenGLProc) ||
        !LoadPlatformExtensions() ||
        !LoadExtensions()
    );
}

bool OpenGLLoader::IsSupported(const char* extension_name) {
    return strstr((const char*)glGetString(GL_EXTENSIONS), extension_name);
}

Version OpenGLLoader::OpenGLVersion(){
    Version result = {0};
    const char *version = (const char *)glGetString(GL_VERSION);
    sscanf(version,"%hu.%hu.%u",&result.Major,&result.Minor, &result.Patch);
    return result;
}

bool OpenGLLoader::LoadExtensions() {
    //TODO: Implement
    return true;
}


