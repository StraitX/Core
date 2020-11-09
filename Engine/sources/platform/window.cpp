#include "platform/window.hpp"

namespace StraitX{

Window::Window():
    m_Impl()
{}

Window::Window(Window &&other):
    m_Impl((WindowImpl&&)(other))
{

}

Window::~Window(){
    m_Impl.Close();
}

Error Window::Open(int width, int height){
    return m_Impl.Open(width,height);
}

Error Window::Close(){
    return m_Impl.Close();
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