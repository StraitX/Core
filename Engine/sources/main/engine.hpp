#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/result.hpp"
#include "servers/display_server.hpp"
#include "main/application.hpp"

int main();

namespace StraitX{

class Application;

class Engine{
private:
    static Engine *s_Instance;

    Application *m_Application = nullptr;
    ApplicationConfig m_ApplicationConfig = {};
    bool m_Running = true;
    Result m_ErrorWindowSystem = Result::None;
    Result m_Window = Result::None;
	Result M_ErrorGraphicsAPI = Result::None;
	Result m_ErrorGraphicsContext = Result::None;
    Result m_ErrorApplication = Result::None;
	Result m_ErrorMX = Result::None;

    u64 m_AllocCalls = 0;
    u64 m_PrevAllocCalls = 0;
    u64 m_FrameAllocCalls = 0;
    u64 m_FreeCalls = 0;
    u64 m_PrevFreeCalls = 0;
    u64 m_FrameFreeCalls = 0;

	friend int ::main();
private:
    Engine();

    ~Engine();

    int Run();
public:

    void Stop();

    u64 FrameAllocCalls()const{
        return m_FrameAllocCalls;
    }

    u64 FrameFreeCalls()const{
        return m_FrameFreeCalls;
    }

    static Engine &Get();
    
private:
    Result Initialize();

    Result Finalize();

    void MainLoop();
};

SX_INLINE Engine &Engine::Get(){
    SX_CORE_ASSERT(s_Instance, "for some reason Engine was not created");
    return *s_Instance;
}

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP