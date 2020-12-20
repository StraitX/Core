#include "main/engine.hpp"
#include "main/application.hpp"
#include "core/log.hpp"
#include "platform/display.hpp"
#include "platform/platform.hpp"
#include "platform/io.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Result::Success){return Result::Failure;}
//#define I(call,name) {Result __err = call; Log(name,__err); if(__err!=Result::Success){return Result::Failure;}}
#define SupportAssert(b, name) if(b){ LogInfo(name ": Supported"); }else{ LogError(name ": Is Not Supported"); return Result::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Result StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine():
    m_Running(true),
    m_ErrorApplication(Result::None)
{

}

Engine::~Engine(){

}


int Engine::Run(){
    LogTrace("Engine::Initialize: Begin");

    Result initCode = Initialize();

    Log("Engine::Initialize",initCode);

    LogSeparator();

    if(initCode == Result::Success){
        LogInfo("Engine: Enter Main loop");
        MainLoop();
        LogInfo("Engine: Left Main loop");
    }else{
        LogWarn("Engine: Main loop was skipped due to Initialization failure");
    }

    LogSeparator();

    LogTrace("Engine: Finalize: Begin");

    Result finalizeCode = Finalize();

    Log("Engine::Finalize",finalizeCode);

    if(finalizeCode != Result::Success || initCode != Result::Success)
        return -1;

    return 0;
}

void Engine::Stop(){
    m_Running = false;
}

Result Engine::Initialize(){
    LogTrace("========= First stage init =========");

    LogTrace("RendererAPI::InitializeHardware: Begin");
    m_ErrorRendererHW = Vk::RendererAPI::Instance.InitializeHardware();
    InitAssert("RendererAPI::InitializeHardware",m_ErrorRendererHW);

    LogTrace("========= Second stage init =========");

    PixelFormat pixel = {0};
    pixel.Red = 8;
    pixel.Green = 8;
    pixel.Blue = 8;
    pixel.Alpha = 8;
    pixel.Depth = 24;
    pixel.Stencil = 8;
    pixel.Samples = 4;

    LogTrace("DisplayServer::Initialize: Begin");
    m_ErrorDisplayServer = m_DisplayServer.Initialize(pixel);
    InitAssert("DisplayServer::Initialize", m_ErrorDisplayServer);

    SupportAssert(m_DisplayServer.m_Display.CheckSupport(Display::Ext::DoubleBuffer), "Display::DoubleBuffer");
    SupportAssert(m_DisplayServer.m_Display.CheckSupport(Display::Ext::OpenGLCore), "Display::OpenGL Core");

    LogTrace("RendererAPI::InitializeRender: Begin");
    m_ErrorRendererAPI = Vk::RendererAPI::Instance.InitializeRender(m_DisplayServer.m_Window);
    InitAssert("RendererAPI::InitializeRender",m_ErrorRendererAPI);

    LogTrace("========= Third stage init =========");

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain: Begin");
    m_Application = StraitXMain();
    m_ErrorMX = (m_Application == nullptr ? Result::NullObject : Result::Success);
    InitAssert("StraitXMain",m_ErrorMX);

    m_DisplayServer.m_Window.SetTitle(m_Application->Name());
    
    m_Application->SetEngine(this);

    LogTrace("Application::OnInitialize: Begin");
    m_ErrorApplication = m_Application->OnInitialize();
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Result::Success;
}

Result Engine::Finalize(){

    if(m_ErrorApplication==Result::Success){
        LogTrace("Application::OnFinalize: Begin");
        m_ErrorApplication = m_Application->OnFinalize();
        Log("Application::OnFinalize",m_ErrorApplication);
    }

    if(m_ErrorMX == Result::Success){
        LogTrace("StraitXExit: Begin");
        m_ErrorMX = StraitXExit(m_Application);
        Log("StraitXExit",m_ErrorMX);
    }

    if(m_ErrorRendererAPI == Result::Success){
        LogTrace("RendererAPI::FinalizeRender: Begin");
        m_ErrorRendererAPI = Vk::RendererAPI::Instance.FinalizeRender();
        Log("RendererAPI::FinalizeRender",m_ErrorRendererAPI);
    }

    if(m_ErrorDisplayServer == Result::Success){
        LogTrace("DisplayServer::Finalize: Begin");
        m_ErrorDisplayServer = m_DisplayServer.Finalize();
        Log("DisplayServer::Finalize", m_ErrorDisplayServer);
    }

    if(m_ErrorRendererHW == Result::Success){
        LogTrace("RendererAPI::FinalizeHardware: Begin");
        m_ErrorRendererHW = Vk::RendererAPI::Instance.FinalizeHardware();
        Log("RendererAPI::FinalizeHardware",m_ErrorRendererHW);
    }

    return Result::Success;
}

void Engine::MainLoop(){
    while(m_Running){
        Event e;
        while(m_DisplayServer.m_Window.PollEvent(e)){
            if(e.Type == EventType::WindowClose)
                Stop();
        }
        m_Application->OnUpdate();
    }
}

}; // namespace StraitX::
