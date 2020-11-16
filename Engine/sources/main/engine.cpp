#include "main/engine.hpp"
#include "main/application.hpp"
#include "core/log.hpp"
#include "platform/display.hpp"
#include "platform/platform.hpp"
#include "platform/io.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Error::Success){return Error::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Error StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine():
    m_Running(true),
    m_Window(),
    m_ErrorWindow(Error::None), 
    m_ErrorDisplay(Error::None), 
    m_ErrorApplication(Error::None)
{

}

Engine::~Engine(){

}


int Engine::Run(){
    LogTrace("Engine::Initialize()");

    Error initCode = Initialize();

    Log("Engine::Initialize",initCode);

    LogSeparator();

    if(initCode == Error::Success){
        LogInfo("Engine: Enter Main loop");
        MainLoop();
        LogInfo("Engine: Left Main loop");
    }else{
        LogWarn("Engine: Main loop was skipped due to Initialization failure");
    }

    LogSeparator();

    LogTrace("Engine: Finalize()");

    Error finalizeCode = Finalize();

    Log("Engine::Finalize",finalizeCode);

    if(finalizeCode != Error::Success || initCode != Error::Success)
        return -1;

    return 0;
}

void Engine::Stop(){
    m_Running = false;
}

Error Engine::Initialize(){
    LogTrace("Display::Open()");
    m_ErrorDisplay = Display::Instance().Open();
    InitAssert("Display::Open", m_ErrorDisplay);

    PixelFormat pixel;
    pixel.Red = 8;
    pixel.Green = 8;
    pixel.Blue = 8;
    pixel.Alpha = 8;
    pixel.Depth = 24;
    pixel.Stencil = 8;
    pixel.Samples = 4;
    FBConfig config;

    Error ErrorFBConfig = config.PickDesired(pixel);
    
    InitAssert("FBConfig::PickDesired",ErrorFBConfig);

    LogTrace("Window::Open()");
    m_ErrorWindow = m_Window.Open(1280,720,config);
    InitAssert("Window::Open",m_ErrorWindow);

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain()");
    m_Application = StraitXMain();
    m_ErrorMX = (m_Application == nullptr ? Error::NullObject : Error::Success);
    InitAssert("StraitXMain",m_ErrorMX);

    m_Window.SetTitle(m_Application->Name());
    
    m_Application->SetEngine(this);

    LogTrace("Application::OnInitialize()");
    m_ErrorApplication = m_Application->OnInitialize();
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Error::Success;
}

Error Engine::Finalize(){

    if(m_ErrorApplication==Error::Success){
        LogTrace("Application::OnFinalize()");
        m_ErrorApplication = m_Application->OnFinalize();
        Log("Application::OnFinalize",m_ErrorApplication);
    }

    if(m_ErrorMX == Error::Success){
        LogTrace("StraitXExit()");
        m_ErrorMX = StraitXExit(m_Application);
        Log("StraitXExit",m_ErrorMX);
    }

    if(m_ErrorWindow == Error::Success){
        LogTrace("Window::Close()");
        m_ErrorWindow = m_Window.Close();
        Log("Window::Close",m_ErrorWindow);
    }
    if(m_ErrorDisplay == Error::Success){
        LogTrace("Display::Close()");
        m_ErrorDisplay = Display::Instance().Close();
        Log("Display::Close",m_ErrorDisplay);
    }

    return Error::Success;
}

void Engine::MainLoop(){
    while(m_Running){
        m_Application->OnUpdate();
    }
}

}; // namespace StraitX::
