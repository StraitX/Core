#include "core/subsystem.hpp"
#include "core/log.hpp"

Subsystem::Subsystem(const char *name_literal):
	m_NameLiteral(name_literal)
{}

Result Subsystem::OnInitialize(){
	return Result::Success;
}

void Subsystem::OnFinalize(){
	(void)0;
}

void Subsystem::OnBeginFrame(){
	(void)0;
}

void Subsystem::OnEndFrame(){
	(void)0;
}

void Subsystem::OnUpdate(float dt){
	(void)dt;
}

void Subsystem::OnEvent(const Event &e){
	(void)e;
}

PushArray<Subsystem*, SubsystemsManager::s_MaxSubsystems> SubsystemsManager::s_Subsystems;

void SubsystemsManager::Finalize(){
	for(auto subsystem: s_Subsystems){
		subsystem->OnFinalize();

		LogTrace("SubsystemsManager: %: Finalized", subsystem->Name());
	}
}

void SubsystemsManager::BeginFrame(){
	for(auto subsystem: s_Subsystems)
		subsystem->OnBeginFrame();
}

void SubsystemsManager::EndFrame(){
	for(auto subsystem: s_Subsystems)
		subsystem->OnEndFrame();
}

void SubsystemsManager::Update(float dt){
	for(auto subsystem: s_Subsystems)
		subsystem->OnUpdate(dt);
}

void SubsystemsManager::HandleEvent(const Event &e){
	for(auto subsystem: s_Subsystems)
		subsystem->OnEvent(e);
}

Result SubsystemsManager::Push(Subsystem *subsystem){
	
	auto res = subsystem->OnInitialize();

	if(res == Result::Success){
		LogTrace("SubsystemsManager: %: Initialized", subsystem->Name());
		s_Subsystems.Push(subsystem);
	}else{
		LogError("SubsystemsManager: %: OnInitialize: %", subsystem->Name(), res.Name());
		LogTrace("SubsystemsManager: Finalizing %", subsystem->Name());
		subsystem->OnFinalize();
	}
	return res;
}
