#define GL_SILENCE_DEPRECATION
#import <Cocoa/Cocoa.h>
#include "platform/macos/opengl_context_impl.hpp"

namespace MacOS{

Result OpenGLContextImpl::Create(const WindowImpl &window, const Version &version){
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

    Handle = [[NSOpenGLContext alloc]initWithFormat:(NSOpenGLPixelFormat*)Format shareContext:NULL];

    if(Handle == nil)return Result::Failure;
    
    [(NSOpenGLContext*)Handle setView: (NSView*)window.View];

    return Result::Success;
}

Result OpenGLContextImpl::CreateLegacy(const WindowImpl &window){
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
                                  initWithFormat:(NSOpenGLPixelFormat*)Format
                                  shareContext:NULL];
    return ResultError(Handle == nil);
}

void OpenGLContextImpl::Destroy(){
    DestroyLegacy();
}

void OpenGLContextImpl::DestroyLegacy(){
    [(NSOpenGLContext*)Handle release];
    [(NSOpenGLPixelFormat*)Format release];
}

Result OpenGLContextImpl::MakeCurrent(){
    [(NSOpenGLContext*)Handle makeCurrentContext];
    return ResultError(Handle != [NSOpenGLContext currentContext]);
}

void OpenGLContextImpl::SwapBuffers(){
    [(NSOpenGLContext*)Handle flushBuffer];
}

void OpenGLContextImpl::Resize(u32 width, u32 height){
    [(NSOpenGLContext*)Handle update];
}

}//namespace MacOS::