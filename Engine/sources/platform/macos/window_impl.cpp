#include "platform/macos/window_impl.hpp"

namespace StraitX{
namespace MacOS{

WindowImpl::WindowImpl(WindowImpl &&other){

}

Result WindowImpl::Open(const ScreenImpl &screen, int width, int height){
    return Result::Success;
}

Result WindowImpl::Close(){
    return Result::Success;
}

bool WindowImpl::IsOpen()const{
    return true;
}

void WindowImpl::SetTitle(const char *title){

}

bool WindowImpl::PollEvent(Event &event){
    return false;
}

Size2u WindowImpl::Size()const{
    return {};
}

}//namespace MacOS::
}//namespace StraitX::
