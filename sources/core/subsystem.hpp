#ifndef STRAITX_SUBSYSTEM_HPP
#define STRAITX_SUBSYSTEM_HPP

#include "platform/events.hpp"
#include "platform/result.hpp"
#include "core/push_array.hpp"

class Subsystem{
private:
	friend class SubsystemsManager;

	const char *m_NameLiteral;
protected:
	Subsystem(const char *name_literal);

	virtual Result OnInitialize();

	virtual void OnFinalize();

	virtual void OnBeginFrame();

	virtual void OnEndFrame();

	virtual void OnUpdate(float dt);

	virtual void OnEvent(const Event &e);

	const char *Name()const{
		return m_NameLiteral;
	}
};

class SubsystemsManager{
private:
	static constexpr size_t s_MaxSubsystems = 4;
	static PushArray<Subsystem*, s_MaxSubsystems> m_Subsystems;

	friend class Engine;
private:
	static void Initialize();

	static void Finalize();

	static void BeginFrame();

	static void EndFrame();

	static void Update(float dt);	

	static void HandleEvent(const Event &e);
	//WARNING: XXX: subsystem's lifetime should be as big as the whole engine lifetime
	static void Push(Subsystem *subsystem);
};

#endif//STRAITX_SUBSYSTEM_HPP