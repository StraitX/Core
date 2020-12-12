#include "platform/window.hpp"
#include "platform/display.hpp"

namespace StraitX{

Window::Window(Display &display):
    m_Impl(display.Impl())
{}

Window::Window(Window &&other):
    m_Impl((WindowImpl&&)(other))
{

}

Window::~Window(){
    m_Impl.Close();
}

Error Window::Open(const Screen &screen, int width, int height){
    if(!IsOpen())
        return m_Impl.Open(screen.Impl(), width, height);
    return Error::AlreadyDone;
}

Error Window::Close(){
    if(IsOpen())
        return m_Impl.Close();
    return Error::NullObject;
}

WindowImpl &Window::Impl(){
    return m_Impl;
}

const WindowImpl &Window::Impl()const{
    return m_Impl;
}

bool Window::IsOpen()const{
    return m_Impl.IsOpen();
}

void Window::SetTitle(const char *title){
    if(IsOpen())
        m_Impl.SetTitle(title);
}

void Window::SetSize(int width, int height){
    if(IsOpen())
        m_Impl.SetSize(width,height);
}

bool Window::PollEvent(Event &event){
    return m_Impl.PollEvent(event);
}

};//namespace StraitX::