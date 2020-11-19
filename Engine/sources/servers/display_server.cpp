#include "servers/display_server.hpp"
#include "core/log.hpp"

#define InitAssert(name, error) Log(name, error);if(error != Error::Success){return Error::Failure;}

namespace StraitX{

DisplayServer *DisplayServer::s_DisplayServer = nullptr;


DisplayServer::DisplayServer():
    m_Display(),
    m_Window(m_Display),
    m_ErrDisplay(Error::None),
    m_ErrWindow(Error::None)
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

Error DisplayServer::Initialize(const PixelFormat &format){
    m_ErrDisplay = m_Display.Open();
    InitAssert("Display::Open", m_ErrDisplay);

    Screen defaultScreen = m_Display.MainScreen();

    FBConfig fbconfig;
    Error ErrFB = fbconfig.PickDesired(m_Display, defaultScreen, format);
    InitAssert("FBConfig::PickDesired", ErrFB);

    m_ErrWindow = m_Window.Open(defaultScreen, 1280, 720, fbconfig);
    InitAssert("Window::Open", m_ErrWindow);

    LogTrace("Keyboard::Initialze");
    Keyboard::Initialize(m_Display.Impl());

    LogTrace("Mouse::Initialize");
    Mouse::Initialize(m_Display.Impl());

    return Error::Success;
}

Error DisplayServer::Finalize(){
    if(m_ErrWindow == Error::Success){
        m_ErrWindow = m_Window.Close();
        Log("Window::Close", m_ErrWindow);
    }
    if(m_ErrDisplay == Error::Success){
        m_ErrDisplay = m_Display.Close();
        Log("Display::Close", m_ErrDisplay); 
    }
    return Error::Success;
}

DisplayServer &DisplayServer::Instance(){
    return *s_DisplayServer;
}


}; // namespace StraitX::