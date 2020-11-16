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

Error Window::Open(const Screen &screen, int width, int height, const FBConfig &config){
    if(!IsOpen())
        return m_Impl.Open(screen.Impl(), width, height, config.Impl());
    return Error::AlreadyDone;
}

Error Window::Close(){
    if(IsOpen())
        return m_Impl.Close();
    return Error::NullObject;
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