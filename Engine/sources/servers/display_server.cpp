#include "platform/window_system.hpp"
#include "servers/display_server.hpp"
#include "core/log.hpp"

#define InitAssert(name, error) Log(name, error);if(error != Result::Success){return Result::Failure;}

namespace StraitX{

DisplayServer *DisplayServer::s_DisplayServer = nullptr;


DisplayServer::DisplayServer():
    m_Window(),
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

Result DisplayServer::Initialize(){

    Screen defaultScreen = WindowSystem::MainScreen();

    m_ErrWindow = m_Window.Open(defaultScreen, 1280, 720);
    InitAssert("Window::Open", m_ErrWindow);


    return Result::Success;
}

Result DisplayServer::Finalize(){
    if(m_ErrWindow == Result::Success){
        m_ErrWindow = m_Window.Close();
        Log("Window::Close", m_ErrWindow);
    }
    return Result::Success;
}

DisplayServer &DisplayServer::Instance(){
    return *s_DisplayServer;
}


}; // namespace StraitX::