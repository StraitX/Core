#include <OpenGL/gl.h>
#include <assert.h>
#import <Foundation/Foundation.h>
#include "platform/glloader.hpp"

namespace StraitX{

bool OpenGLLoader::LoadPlatformExtensions(){
    return true;
}

void *OpenGLLoader::GetOpenGLProc(const char *name){

    CFBundleRef nsgl = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
    assert(nsgl); 
    CFStringRef symbol_name = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);
    assert(symbol_name);
    void *symbol = CFBundleGetFunctionPointerForName(nsgl, symbol_name);
    assert(symbol);
    CFRelease(symbol_name);

    return symbol;
}

} // namespace StraitX::