#include "core/os/memory.hpp"
#include "core/os/clock.hpp"
#include "core/log.hpp"
#include "core/print.hpp"
#include "graphics/graphics_api_loader.hpp"
#include "graphics/graphics_context.hpp"
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
    m_AppConfig = GetApplicationConfig();
    SX_CORE_ASSERT(m_AppConfig.ApplicationName, "ApplicationConfig::ApplicationName should be valid string pointer");

    LogTrace("========= First stage init =========");

	Result w = m_RenderWindow.Open(m_AppConfig.WindowSize.x, m_AppConfig.WindowSize.y, m_AppConfig.ApplicationName);
	//XXX
	SX_CORE_ASSERT(w, "Can't open a window");

    LogTrace("GraphicsAPILoader::Load: Begin");
	auto error_api_load = GraphicsAPILoader::Load(m_AppConfig.DesiredAPI);
    InitAssert("GraphicsAPILoader::Load", error_api_load);

	LogTrace("GraphicsContext::New: Begin");
	{
		m_ErrorGraphicsContext = GraphicsContext::Get().Initialize(&m_RenderWindow);
	}
	InitAssert("GraphicsContext::New", m_ErrorGraphicsContext);

    LogTrace("========= Second stage init =========");

	Renderer2D::Register(m_Delegates);
	
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

	if(m_Delegates.OnInitialize.Size()){
		LogTrace("EngineDelegates: OnInitialize: Begin");
		m_Delegates.OnInitialize();
		LogTrace("EngineDelegates: OnInitialize: End");
	}

    return Result::Success;
}

void Engine::Finalize(){
	if(m_Delegates.OnFinalize.Size()){
		LogTrace("EngineDelegates: OnFinalize: Begin");
		for(size_t i = m_Delegates.OnFinalize.Size() - 1; ; i--){
			m_Delegates.OnFinalize[i].operator()();

			if(i == 0)break;
		}
		LogTrace("EngineDelegates: OnFinalize: End");
	}

	m_RenderWindow.Close();

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

	if(m_ErrorGraphicsContext == Result::Success){
		LogTrace("GraphicsContext::Finalize: Begin");
		GraphicsContext::Get().Finalize();
		LogTrace("GraphicsContext::Finalize: End");
	}
}

bool Engine::Tick(float dt){
	m_Delegates.OnBeginFrame();

	m_Delegates.OnUpdate(dt);
	m_Application->OnUpdate(dt);

	m_Delegates.OnEndFrame();

	GraphicsContext::Get().SwapBuffers();


	m_AllocCalls = Memory::AllocCalls();
	m_FrameAllocCalls = m_AllocCalls - m_PrevAllocCalls;
	m_PrevAllocCalls = m_AllocCalls;

	m_FreeCalls = Memory::FreeCalls();
	m_FrameFreeCalls = m_FreeCalls - m_PrevFreeCalls;
	m_PrevFreeCalls = m_FreeCalls;

	return m_Running && m_RenderWindow.IsOpen();
}

void Engine::HandleEvent(const Event &e){
	if(e.Type == EventType::WindowClose)
		Stop();

	if(e.Type == EventType::WindowResized)
		GraphicsContext::Get().ResizeSwapchain(e.WindowResized.x, e.WindowResized.y);

	m_Application->OnEvent(e);
	m_Delegates.OnEvent(e);
}

