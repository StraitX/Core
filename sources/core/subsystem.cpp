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

PushArray<Subsystem*, SubsystemsManager::s_MaxSubsystems> SubsystemsManager::m_Subsystems;

void SubsystemsManager::Initialize(){
	for(size_t i = 0; i<m_Subsystems.Size(); i++){
		auto res = m_Subsystems[i]->OnInitialize();

		if(res == Result::Success){
			LogTrace("SubsystemsManager: %: Initialized", m_Subsystems[i]->Name());
		}else{
			LogError("SubsystemsManager: %: OnInitialize: %", m_Subsystems[i]->Name(), res.Name());
			LogTrace("SubsystemsManager: Finalizing %", m_Subsystems[i]->Name());
			m_Subsystems[i]->OnFinalize();
			m_Subsystems[i] = m_Subsystems[m_Subsystems.Size() - 1];
			m_Subsystems.Pop();
			--i;
		}
	}
}

void SubsystemsManager::Finalize(){
	for(auto subsystem: m_Subsystems){
		subsystem->OnFinalize();

		LogTrace("SubsystemsManager: %: Finalized", subsystem->Name());
	}
}

void SubsystemsManager::BeginFrame(){
	for(auto subsystem: m_Subsystems)
		subsystem->OnBeginFrame();
}

void SubsystemsManager::EndFrame(){
	for(auto subsystem: m_Subsystems)
		subsystem->OnEndFrame();
}

void SubsystemsManager::Update(float dt){
	for(auto subsystem: m_Subsystems)
		subsystem->OnUpdate(dt);
}

void SubsystemsManager::HandleEvent(const Event &e){
	for(auto subsystem: m_Subsystems)
		subsystem->OnEvent(e);
}

void SubsystemsManager::Push(Subsystem *subsystem){
	m_Subsystems.Push(subsystem);
}
