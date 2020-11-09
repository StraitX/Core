#include "main/engine.hpp"
#include "main/application.hpp"
#include "core/log.hpp"
#include "platform/platform.hpp"
#include "platform/io.hpp"


#define InitAssert(code,message) if(!(code == ErrorCode::Success)){LogError(message); return ErrorCode::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Error StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine():
    m_Running(true),
    m_Window(),
    m_ErrorWindow(ErrorCode::None), 
    m_ErrorPlatform(ErrorCode::None), 
    m_ErrorApplication(ErrorCode::None)
{

}

Engine::~Engine(){

}


Error Engine::Run(){
    LogTrace("Engine::Initialize()");

    Error initCode = Initialize();

    if(initCode != ErrorCode::Success){
        LogError("Engine::Initialize: Failed");
        LogWarn("Engine: Force Finalizing");
    }else{
        LogInfo("Engine::Initialize: Finished");
    }
    LogSeparator();

    if(initCode == ErrorCode::Success){
        LogInfo("Engine: Enter Main loop");
        MainLoop();
        LogInfo("Engine: Left Main loop");
    }else{
        LogWarn("Engine: Main loop was skipped due to Initialization failure");
    }

    LogSeparator();

    LogTrace("Engine: Finalize()");
    Error finalizeCode = Finalize();

    if(finalizeCode==ErrorCode::Success){
        LogInfo("Engine::Finalize: Success");
    }else{
        LogError("Engine::Finalize: Failed");
    }

    if(finalizeCode != ErrorCode::Success || initCode != ErrorCode::Success)
        return ErrorCode::Failure;

    return 0;
}

void Engine::Stop(){
    m_Running = false;
}

Error Engine::Initialize(){
    LogTrace("Platform::Initialize()");
    m_ErrorPlatform = Platform::Initialize();
    InitAssert(m_ErrorPlatform,"Platform::Initialize: Failed");

    LogTrace("Window::Open()");
    m_ErrorWindow = m_Window.Open(1280,720);
    InitAssert(m_ErrorWindow,"Window::Open: Can't open a window");

    LogTrace("StraitXMain()");
    //Engine should be completely initialized at this moment
    m_Application = StraitXMain();
    m_ErrorMX = m_Application == nullptr ? ErrorCode::Failure : ErrorCode::Success;
    InitAssert(m_ErrorMX,"StraitXMain: Application was not provided");

    m_Window.SetTitle(m_Application->Name());
    
    m_Application->SetEngine(this);
    LogTrace("Application::OnInitialize()");
    m_ErrorApplication = m_Application->OnInitialize();
    InitAssert(m_ErrorApplication,"Application::OnInitialize: Failed");

    return ErrorCode::Success;
}

Error Engine::Finalize(){

    if(m_ErrorApplication==ErrorCode::Success){
        LogTrace("Application::OnFinalize()");
        m_ErrorApplication = m_Application->OnFinalize();
        InitAssert(m_ErrorApplication,"Application::OnFinalize: Failed");
    }

    if(m_ErrorMX == ErrorCode::Success){
        LogTrace("StraitXExit()");
        m_ErrorMX = StraitXExit(m_Application);
        InitAssert(m_ErrorMX,"StraitXExit: returned Failure");
    }

    if(m_ErrorWindow == ErrorCode::Success){
        LogTrace("Window::Close()");
        m_ErrorWindow = m_Window.Close();
        InitAssert(m_ErrorWindow,"Window::Close: Failed");
    }
    if(m_ErrorPlatform == ErrorCode::Success){
        LogTrace("Platform::Finalize()");
        m_ErrorPlatform = Platform::Finalize();
        InitAssert(m_ErrorPlatform,"Platform::Finalize: Failed");
    }

    return ErrorCode::Success;
}

void Engine::MainLoop(){
    while(m_Running){
        m_Application->OnUpdate();
    }
}

}; // namespace StraitX::
