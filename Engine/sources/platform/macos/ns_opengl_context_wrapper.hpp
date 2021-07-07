#ifndef STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP
#define STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP

#include "platform/macos/common.hpp"
#import <Cocoa/Cocoa.h>
#include "platform/result.hpp"
#include "platform/window.hpp"

namespace MacOS{

struct NSOpenGLContextWrapper{
    NSOpenGLContext *Handle;
    NSOpenGLPixelFormat *Format;

    Result Create(const WindowImpl &window, const Version &version);

    Result CreateLegacy();

    void Destroy();

    void DestroyLegacy();

    Result MakeCurrent();

    void SwapBuffers();

    void Resize(u32 width, u32 height);
};

}//namespace MacOS::

#endif// STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP