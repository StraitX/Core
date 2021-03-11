#include "platform/macos/common.hpp"
#include "platform/macos/opengl_context_impl.hpp"
#include "platform/macos/ns_opengl_context_wrapper.hpp"

namespace StraitX{
namespace MacOS{

Result OpenGLContextImpl::Create(WindowImpl &window, const Version &version){
    auto *ctx = new NSOpenGLContextWrapper();
    Handle = ctx;
    return ctx->Create(window, version);
}

Result OpenGLContextImpl::CreateDummy(){
    auto *ctx = new NSOpenGLContextWrapper();
    Handle = ctx;
    return ctx->CreateDummy();
}

void OpenGLContextImpl::Destroy(){
    auto *ctx = static_cast<NSOpenGLContextWrapper*>(Handle);
    ctx->DestroyDummy();
    delete ctx;
}

void OpenGLContextImpl::DestroyDummy(){
    auto *ctx = static_cast<NSOpenGLContextWrapper*>(Handle);
    ctx->DestroyDummy();
    delete ctx;
}

Result OpenGLContextImpl::MakeCurrent(){
    return static_cast<NSOpenGLContextWrapper*>(Handle)->MakeCurrent();
}

void OpenGLContextImpl::SwapBuffers(){
    static_cast<NSOpenGLContextWrapper*>(Handle)->SwapBuffers();
}

}//namespace MacOS::
}//namespace StraitX::
