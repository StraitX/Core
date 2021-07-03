#include "main/engine.hpp"
#include "platform/platform_runtime.hpp"

Result PlatformRuntime::Initialize(){
	static Engine s_Engine;

	return Engine::s_Instance->Initialize();
}

bool PlatformRuntime::Tick(float dt){
	return Engine::s_Instance->Tick(dt);
}

void PlatformRuntime::ProcessEvent(const Event &e){
	Engine::s_Instance->ProcessEvent(e);
}

void PlatformRuntime::Finalize(){
	Engine::s_Instance->Finalize();
}