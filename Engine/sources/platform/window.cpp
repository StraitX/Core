#include "platform/window.hpp"

namespace StraitX{

Window::Window(int width, int height, const char *title):
    mImpl(width,height)
{
    SetTitle(title);
}

Window::~Window(){

}


void Window::SetTitle(const char *title){
    mImpl.SetTitle(title);
}

void Window::SetSize(int width, int height){
    mImpl.SetSize(width,height);
}

bool Window::PollEvent(Event &event){
    return mImpl.PollEvent(event);
}

};//namespace StraitX::