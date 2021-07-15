#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP

#include "platform/result.hpp"
#include "platform/platform_runtime.hpp"
#include "main/application.hpp"

class Application;

class Engine{
private:
    static Engine *s_Instance;

    Application *m_Application = nullptr;
    ApplicationConfig m_ApplicationConfig = {};
    bool m_Running = true;
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

	friend class PlatformRuntime;
private:
    Engine();

    ~Engine();
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

	bool Tick(float dt);

	void HandleEvent(const Event &e);

    void Finalize();
};

SX_INLINE Engine &Engine::Get(){
    SX_CORE_ASSERT(s_Instance, "for some reason Engine was not created");
    return *s_Instance;
}

#endif // STRAITX_ENGINE_HPP