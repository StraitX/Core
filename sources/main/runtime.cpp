#include "main/engine.hpp"
#include "core/os/platform_runtime.hpp"

Result PlatformRuntime::Initialize(){
	static Engine s_Engine;

	return Engine::s_Instance->Initialize();
}

bool PlatformRuntime::Tick(float dt){
	return Engine::s_Instance->Tick(dt);
}

void PlatformRuntime::HandleEvent(const Event &e){
	// some platforms can post events during Window creation, during that process application is nullptr
	if(Engine::s_Instance && Engine::s_Instance->m_Application)
		Engine::s_Instance->HandleEvent(e);
}

void PlatformRuntime::Finalize(){
	Engine::s_Instance->Finalize();
}