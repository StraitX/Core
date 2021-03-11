#ifndef STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP
#define STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP

#include "platform/macos/common.hpp"
#import <Cocoa/Cocoa.h>
#include "platform/result.hpp"
#include "platform/window.hpp"

namespace StraitX{
namespace MacOS{

struct NSOpenGLContextWrapper{
    NSOpenGLContext *Handle;
    NSOpenGLPixelFormat *Format;

    Result Create(const WindowImpl &window, const Version &version);

    Result CreateDummy();

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();
};

}//namespace MacOS::
}//namespace StraitX::

#endif// STRAITX_MACOS_NS_OPENGL_CONTEXT_WRAPPER_HPP