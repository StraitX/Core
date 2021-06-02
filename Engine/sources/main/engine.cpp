#include "platform/window_system.hpp"
#include "platform/io.hpp"
#include "platform/memory.hpp"
#include "platform/clock.hpp"
#include "core/log.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/api/gpu_context.hpp"
#include "graphics/api/graphics_api_loader.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Result::Success){return Result::Failure;}
//#define I(call,name) {Result __err = call; Log(name,__err); if(__err!=Result::Success){return Result::Failure;}}
#define SupportAssert(b, name) if(b){ LogInfo(name ": Supported"); }else{ LogError(name ": Is Not Supported"); return Result::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern void StraitXExit(StraitX::Application *);

namespace StraitX{

Engine *Engine::s_Instance = nullptr;

Engine::Engine(){
    if(!s_Instance)
        s_Instance = this;
#ifdef SX_DEBUG
    else SX_CORE_ASSERT(false, "Engine: created twice");
#endif
}

Engine::~Engine(){
    if(s_Instance == this)
        s_Instance = nullptr;
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
    m_ApplicationConfig = GetApplicationConfig();
    SX_CORE_ASSERT(m_ApplicationConfig.ApplicationName, "ApplicationConfig::ApplicationName should be valid string pointer");

    LogTrace("========= First stage init =========");

    LogInfo("WindowSystem::Initialize: Begin");
    {
        m_ErrorWindowSystem = WindowSystem::Initialize();
    }
    InitAssert("WindowSystem::Initialize", m_ErrorWindowSystem);

    LogTrace("Window::Open: Begin");
    {
        m_Window = DisplayServer::Window.Open(WindowSystem::MainScreen(), m_ApplicationConfig.WindowSize.x, m_ApplicationConfig.WindowSize.y, m_ApplicationConfig.DesiredAPI, m_ApplicationConfig.SwapchainProps);
    }
    InitAssert("Window::Open", m_Window);

    DisplayServer::Window.SetTitle(m_ApplicationConfig.ApplicationName);

    LogTrace("========= Second stage init =========");

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain: Begin");
    {
        m_Application = StraitXMain();
        m_ErrorMX = (m_Application == nullptr ? Result::NullObject : Result::Success);
    }
    InitAssert("StraitXMain",m_ErrorMX);
    
    m_Application->SetEngine(this);

    LogTrace("Application::OnInitialize: Begin");
    {
        m_ErrorApplication = m_Application->OnInitialize();
    }
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Result::Success;
}

Result Engine::Finalize(){

    if(m_ErrorApplication==Result::Success){
        LogTrace("Application::OnFinalize: Begin");
        m_Application->OnFinalize();
        LogTrace("Application::OnFinalize: End");
    }

    if(m_ErrorMX == Result::Success){
        LogTrace("StraitXExit: Begin");
        StraitXExit(m_Application);
        LogTrace("StraitXExit: End");
    }

    if(m_Window == Result::Success){
        LogTrace("DisplayServer::Finalize: Begin");
        DisplayServer::Window.Close();
        LogTrace("DisplayServer::Finalize: End");
    }

    if(m_ErrorWindowSystem == Result::Success){
        LogTrace("WindowSystem::Finalize: Begin");
        m_ErrorWindowSystem = WindowSystem::Finalize();
        Log("WindowSystem::Finalize",m_ErrorWindowSystem);
    }


    return Result::Success;
}

void Engine::MainLoop(){
    Clock frametime;
    while(m_Running){
        Event e;
        while(DisplayServer::Window.PollEvent(e)){
            if(e.Type == EventType::WindowClose)
                Stop();
            else
                (void)m_Application->OnEvent(e);
        }
        float dt = frametime.GetElapsedTime().AsSeconds();
        frametime.Restart();

        m_Application->OnUpdate(dt);

        m_AllocCalls = Memory::AllocCalls();
        m_FrameAllocCalls = m_AllocCalls - m_PrevAllocCalls;
        m_PrevAllocCalls = m_AllocCalls;

        m_FreeCalls = Memory::FreeCalls();
        m_FrameFreeCalls = m_FreeCalls - m_PrevFreeCalls;
        m_PrevFreeCalls = m_FreeCalls;

    }
}

}; // namespace StraitX::
