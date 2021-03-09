#include "platform/macos/opengl_context_impl.hpp"

namespace StraitX{
namespace MacOS{

Result OpenGLContextImpl::Create(WindowImpl &window, const Version &version){
    return Result::Success;
}

Result OpenGLContextImpl::CreateDummy(){
    return Result::Success;
}

void OpenGLContextImpl::Destroy(){

}

void OpenGLContextImpl::DestroyDummy(){

}

Result OpenGLContextImpl::MakeCurrent(){
    return Result::Success;
}

void OpenGLContextImpl::SwapBuffers(){

}

}//namespace MacOS::
}//namespace StraitX::
