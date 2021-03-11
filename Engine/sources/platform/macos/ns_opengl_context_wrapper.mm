#include "platform/macos/ns_opengl_context_wrapper.hpp"

namespace StraitX{
namespace MacOS{

Result NSOpenGLContextWrapper::Create(const WindowImpl &window, const Version &version){
    return CreateDummy();
}

Result NSOpenGLContextWrapper::CreateDummy(){
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFASampleBuffers, 0,
        0,
    };

    Format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

    if(Format == nil)
        return Result::Unsupported;

    Handle = [[NSOpenGLContext alloc]
                                  initWithFormat:Format
                                  shareContext:NULL];
    return ResultError(Handle == nil);
}

void NSOpenGLContextWrapper::Destroy(){
    DestroyDummy();
}

void NSOpenGLContextWrapper::DestroyDummy(){
    [Handle release];
    [Format release];
}

Result NSOpenGLContextWrapper::MakeCurrent(){
    [Handle makeCurrentContext];
    return ResultError(Handle != [NSOpenGLContext currentContext]);
}

void NSOpenGLContextWrapper::SwapBuffers(){
    [Handle flushBuffer];
}

}//namespace MacOS::
}//namespace StraitX::