#include "platform/macos/common.hpp"
#include "platform/macos/ns_opengl_context_wrapper.hpp"
#include "platform/macos/sx_window.hpp"

namespace MacOS{

Result NSOpenGLContextWrapper::Create(const WindowImpl &window, const Version &version){
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAAccelerated,
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

    Handle = [[NSOpenGLContext alloc]initWithFormat:Format shareContext:NULL];

    if(Handle == nil)return Result::Failure;

    assert(Handle);
    auto *wrapper = static_cast<SXWindowWrapper*>(window.Handle);
    assert(wrapper);
    
    [Handle setView: wrapper->View];

    return Result::Success;
}

Result NSOpenGLContextWrapper::CreateLegacy(){
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
    DestroyLegacy();
}

void NSOpenGLContextWrapper::DestroyLegacy(){
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

void NSOpenGLContextWrapper::Resize(u32 width, u32 height){
    [Handle update];
}

}//namespace MacOS::