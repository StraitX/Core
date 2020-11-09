#include "platform/window.hpp"

namespace StraitX{

Window::Window(int width, int height, const char *title):
    m_Impl(width,height)
{
    SetTitle(title);
}

Window::Window(Window &&other):
    m_Impl((WindowImpl&&)(other))
{

}

Window::~Window(){

}

bool Window::IsOpen(){
    return m_Impl.IsOpen();
}

void Window::SetTitle(const char *title){
    m_Impl.SetTitle(title);
}

void Window::SetSize(int width, int height){
    m_Impl.SetSize(width,height);
}

bool Window::PollEvent(Event &event){
    return m_Impl.PollEvent(event);
}

};//namespace StraitX::