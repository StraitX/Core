#include "platform/macos/window_impl.hpp"
#include "platform/macos/sx_window.hpp"

namespace MacOS{

Result WindowImpl::Open(const ScreenImpl &screen, int width, int height){
    auto *handle = new SXWindowWrapper();
    Handle = handle;
    return handle->Open(screen, width, height);
}

Result WindowImpl::Close(){
    auto *handle = static_cast<SXWindowWrapper*>(Handle);
    auto res = handle->Close();
    delete handle;
    return res;
}

bool WindowImpl::IsOpen()const{
    return static_cast<SXWindowWrapper*>(Handle)->IsOpen();
}

void WindowImpl::SetTitle(const char *title){
    static_cast<SXWindowWrapper*>(Handle)->SetTitle(title);
}

bool WindowImpl::PollEvent(Event &event){
    return static_cast<SXWindowWrapper*>(Handle)->PollEvent(event);
}

Size2u WindowImpl::Size()const{
    return static_cast<SXWindowWrapper*>(Handle)->Size();
}

void WindowImpl::SetSize(u32 width, u32 height){
    static_cast<SXWindowWrapper*>(Handle)->SetSize(width, height);
}

}//namespace MacOS::