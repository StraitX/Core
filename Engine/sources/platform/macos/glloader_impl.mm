#include "platform/macos/common.hpp"
#include <OpenGL/gl.h>
#include <assert.h>
#import <Foundation/Foundation.h>
#include "platform/glloader.hpp"

bool OpenGLLoader::LoadPlatformExtensions(){
    return true;
}

void *OpenGLLoader::GetOpenGLProc(const char *name){
    //TODO: XXX: check this somewhere
    //TODO: XXX: deal with memory allocations
    CFBundleRef nsgl = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    assert(nsgl); 

    CFStringRef symbol_name = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);

    void *symbol = CFBundleGetFunctionPointerForName(nsgl, symbol_name);
    CFRelease(symbol_name);
    return symbol;
}