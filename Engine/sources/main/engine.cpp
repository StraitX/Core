#include "main/engine.hpp"
#include "main/application.hpp"
#include "core/log.hpp"
#include "platform/display.hpp"
#include "platform/platform.hpp"
#include "platform/io.hpp"
#include "platform/glloader.hpp"
#include "platform/glcontext.hpp"
#include "platform/opengl.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Error::Success){return Error::Failure;}
//#define I(call,name) {Error __err = call; Log(name,__err); if(__err!=Error::Success){return Error::Failure;}}
#define SupportAssert(b, name) if(b){ LogInfo(name ": Supported"); }else{ LogError(name ": Is Not Supported"); return Error::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Error StraitXExit(StraitX::Application *);

namespace StraitX{


Engine::Engine():
    m_Running(true),
    m_Context(m_DisplayServer.m_Window),
    m_ErrorApplication(Error::None)
{

}

Engine::~Engine(){

}


int Engine::Run(){
    LogTrace("Engine::Initialize: Begin");

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

    LogTrace("Engine: Finalize: Begin");

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
    LogTrace("========= First stage init =========");
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

    LogTrace("========= Second stage init =========");

    SupportAssert(m_DisplayServer.m_Display.CheckSupport(Display::Ext::DoubleBuffer), "Display::DoubleBuffer");
    SupportAssert(m_DisplayServer.m_Display.CheckSupport(Display::Ext::OpenGLCore), "Display::OpenGL Core");

    Error ErrorContext = m_Context.Create(m_DisplayServer.m_Window.GetFBConfig(), {4,6,0});
    InitAssert("OpenGL Context::Create", ErrorContext);

    InitAssert("OpenGL Context::MakeCurrent",m_Context.MakeCurrent());

    Error ErrorOpenGL = OpenGLLoader::Load();
    InitAssert("OpenGL Loader::Load", ErrorOpenGL);

    auto glVersion = OpenGLLoader::OpenGLVersion();
    Output::Printf("OpenGL Loader: OpenGL %\n",glVersion);

    Output::Printf("OpenGL Renderer: %\n", (const char *)glGetString(GL_RENDERER));
    Output::Printf("OpenGL Version : %\n", (const char *)glGetString(GL_VERSION));
    Output::Printf("OpenGL Vendor  : %\n", (const char *)glGetString(GL_VENDOR));

    LogTrace("========= Third stage init =========");

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain: Begin");
    m_Application = StraitXMain();
    m_ErrorMX = (m_Application == nullptr ? Error::NullObject : Error::Success);
    InitAssert("StraitXMain",m_ErrorMX);

    m_DisplayServer.m_Window.SetTitle(m_Application->Name());
    
    m_Application->SetEngine(this);

    LogTrace("Application::OnInitialize: Begin");
    m_ErrorApplication = m_Application->OnInitialize();
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Error::Success;
}

Error Engine::Finalize(){

    if(m_ErrorApplication==Error::Success){
        LogTrace("Application::OnFinalize: Begin");
        m_ErrorApplication = m_Application->OnFinalize();
        Log("Application::OnFinalize",m_ErrorApplication);
    }

    if(m_ErrorMX == Error::Success){
        LogTrace("StraitXExit: Begin");
        m_ErrorMX = StraitXExit(m_Application);
        Log("StraitXExit",m_ErrorMX);
    }

    if(m_ErrorDisplayServer == Error::Success){
        LogTrace("DisplayServer::Finalize: Begin");
        m_ErrorDisplayServer = m_DisplayServer.Finalize();
        Log("DisplayServer::Finalize", m_ErrorDisplayServer);
    }

    return Error::Success;
}

void Engine::MainLoop(){
    while(m_Running){
        Event e;
        while(m_DisplayServer.m_Window.PollEvent(e)){
            if(e.Type == EventType::WindowClose)
                Stop();
        }
        m_Application->OnUpdate();
        m_Context.SwapBuffers();
    }
}

}; // namespace StraitX::
