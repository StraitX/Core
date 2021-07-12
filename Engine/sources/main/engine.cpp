#include "platform/window_system.hpp"
#include "platform/memory.hpp"
#include "platform/clock.hpp"
#include "core/log.hpp"
#include "core/subsystem.hpp"
#include "core/print.hpp"
#include "graphics/api/graphics_api_loader.hpp"
#include "graphics/api/graphics_context.hpp"
#include "graphics/imgui/imgui_backend.hpp"
#include "graphics/renderer_2d.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Result::Success){return Result::Failure;}
//#define I(call,name) {Result __err = call; Log(name,__err); if(__err!=Result::Success){return Result::Failure;}}
#define SupportAssert(b, name) if(b){ LogInfo(name ": Supported"); }else{ LogError(name ": Is Not Supported"); return Result::Failure;}

// should be defined at client side
extern Application *StraitXMain();
extern void StraitXExit(Application *);


Engine *Engine::s_Instance = nullptr;

Engine::Engine(){
	s_Instance = this;
}

Engine::~Engine(){
	s_Instance = nullptr;
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
        m_ErrorWindowSystem = WindowSystem::Initialize(m_ApplicationConfig.WindowSize.x, m_ApplicationConfig.WindowSize.y);
    }
    InitAssert("WindowSystem::Initialize", m_ErrorWindowSystem);

	WindowSystem::Window().SetTitle(m_ApplicationConfig.ApplicationName);

	Println("Size: %: %", WindowSystem::Window().Size().width, WindowSystem::Window().Size().height);

    LogTrace("GraphicsAPILoader::Load: Begin");
	auto error_api_load = GraphicsAPILoader::Load(m_ApplicationConfig.DesiredAPI);
    InitAssert("GraphicsAPILoader::Load", error_api_load);

	LogTrace("GraphicsContext::New: Begin");
	{
		m_ErrorGraphicsContext = GraphicsContext::Get().Initialize(WindowSystem::Window());
	}
	InitAssert("GraphicsContext::New", m_ErrorGraphicsContext);

    LogTrace("========= Second stage init =========");

	static Renderer2D renderer_2d;
	static ImGuiBackend imgui_backend;
	SubsystemsManager::Push(&renderer_2d);
	SubsystemsManager::Push(&imgui_backend);

	LogTrace("SubsystemsManager::Initialize: Begin");
	{
		SubsystemsManager::Initialize();
	}
	LogTrace("SubsystemsManager::Initialize: End");

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain: Begin");
    {
        m_Application = StraitXMain();
        m_ErrorMX = (m_Application == nullptr ? Result::NullObject : Result::Success);
    }
    InitAssert("StraitXMain",m_ErrorMX);

    LogTrace("Application::OnInitialize: Begin");
    {
        m_ErrorApplication = m_Application->OnInitialize();
    }
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Result::Success;
}

void Engine::Finalize(){

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

	LogTrace("SubsystemsManager::Finalize: Begin");
	{
		SubsystemsManager::Finalize();
	}
	LogTrace("SubsystemsManager::Finalize: End");

	if(m_ErrorGraphicsContext == Result::Success){
		LogTrace("GraphicsContext::Finalize: Begin");
		GraphicsContext::Get().Finalize();
		LogTrace("GraphicsContext::Finalize: End");
	}

    if(m_ErrorWindowSystem == Result::Success){
        LogTrace("WindowSystem::Finalize: Begin");
        WindowSystem::Finalize();
        Log("WindowSystem::Finalize",m_ErrorWindowSystem);
    }
}

bool Engine::Tick(float dt){
	SubsystemsManager::BeginFrame();

	SubsystemsManager::Update(dt);
	m_Application->OnUpdate(dt);

	SubsystemsManager::EndFrame();

	GraphicsContext::Get().SwapBuffers();


	m_AllocCalls = Memory::AllocCalls();
	m_FrameAllocCalls = m_AllocCalls - m_PrevAllocCalls;
	m_PrevAllocCalls = m_AllocCalls;

	m_FreeCalls = Memory::FreeCalls();
	m_FrameFreeCalls = m_FreeCalls - m_PrevFreeCalls;
	m_PrevFreeCalls = m_FreeCalls;

	return m_Running;
}

void Engine::ProcessEvent(const Event &e){
	if(e.Type == EventType::WindowClose)
		Stop();

	if(e.Type == EventType::WindowResized)
		GraphicsContext::Get().ResizeSwapchain(e.WindowResized.x, e.WindowResized.y);

	m_Application->OnEvent(e);
	SubsystemsManager::ProcessEvent(e);
}

