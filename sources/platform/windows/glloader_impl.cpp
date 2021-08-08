#include <Windows.h>
#include <string.h>
#include "core/os/glloader.hpp"
#include "platform/windows/wgl.hpp"

bool OpenGLLoader::LoadPlatformExtensions() {
    Windows::wglChoosePixelFormatARB = (Windows::wglChoosePixelFormatARBProc)wglGetProcAddress("wglChoosePixelFormatARB");
    Windows::wglCreateContextAttribsARB = (Windows::wglCreateContextAttribsARBProc)wglGetProcAddress("wglCreateContextAttribsARB");
    Windows::wglGetPixelFormatAttribivARB = (Windows::wglGetPixelFormatAttribivARBProc)wglGetProcAddress("wglGetPixelFormatAttribivARB");
    return Windows::wglChoosePixelFormatARB && Windows::wglCreateContextAttribsARB && Windows::wglGetPixelFormatAttribivARB;
}

void *OpenGLLoader::GetOpenGLProc(const char *name){
    auto result = reinterpret_cast<void*>(wglGetProcAddress((LPCSTR)name));
    if (result)
        return result;
    return GetProcAddress((HINSTANCE)GetModuleHandle("opengl32.dll"),(LPCSTR)name);
}