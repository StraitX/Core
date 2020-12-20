#include "servers/display_server.hpp"
#include "core/log.hpp"

#define InitAssert(name, error) Log(name, error);if(error != Result::Success){return Result::Failure;}

namespace StraitX{

DisplayServer *DisplayServer::s_DisplayServer = nullptr;


DisplayServer::DisplayServer():
    m_Display(),
    m_Window(m_Display),
    m_ErrDisplay(Result::None),
    m_ErrWindow(Result::None)
{
    if(s_DisplayServer==nullptr){
        s_DisplayServer = this;
    }else{
        LogWarn("DisplayServer: Created shallow copy");
    }
}

DisplayServer::~DisplayServer(){
    if(s_DisplayServer == this){
        s_DisplayServer = nullptr;
    }else{
        LogWarn("DisplayServer: Deleted shallow copy");
    }
}

Result DisplayServer::Initialize(const PixelFormat &format){
    m_ErrDisplay = m_Display.Open();
    InitAssert("Display::Open", m_ErrDisplay);

    Screen defaultScreen = m_Display.MainScreen();

    m_ErrWindow = m_Window.Open(defaultScreen, 1280, 720);
    InitAssert("Window::Open", m_ErrWindow);

    LogTrace("Keyboard::Initialze");
    Keyboard::Initialize(m_Display.Impl());

    LogTrace("Mouse::Initialize");
    Mouse::Initialize(m_Display.Impl());

    return Result::Success;
}

Result DisplayServer::Finalize(){
    if(m_ErrWindow == Result::Success){
        m_ErrWindow = m_Window.Close();
        Log("Window::Close", m_ErrWindow);
    }
    if(m_ErrDisplay == Result::Success){
        m_ErrDisplay = m_Display.Close();
        Log("Display::Close", m_ErrDisplay); 
    }
    return Result::Success;
}

DisplayServer &DisplayServer::Instance(){
    return *s_DisplayServer;
}


}; // namespace StraitX::