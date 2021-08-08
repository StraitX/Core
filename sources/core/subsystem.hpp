#ifndef STRAITX_SUBSYSTEM_HPP
#define STRAITX_SUBSYSTEM_HPP

#include "core/os/events.hpp"
#include "core/result.hpp"
#include "core/push_array.hpp"
#include "core/assert.hpp"

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
	static constexpr size_t s_MaxSubsystems = 8;

	static PushArray<Subsystem*, s_MaxSubsystems> s_Subsystems;

	friend class Engine;
private:
	static void Finalize();

	static void BeginFrame();

	static void EndFrame();

	static void Update(float dt);	

	static void HandleEvent(const Event &e);

	//WARNING: XXX: subsystem's lifetime should be as big as the whole engine lifetime
	static Result Push(Subsystem *subsystem);
public:
	template<typename T>
	static Result Register(){
		static_assert(IsBaseOf<Subsystem, T>::Value, "SubsystemManager: Can't insert non-subsystem class");

		static T subsystem;
#ifdef SX_DEBUG
		static bool is_registered = false;

		SX_CORE_ASSERT(!is_registered, "SubsystemManager: can't register one subsystem twice");

		is_registered = true;
#endif
		return Push(&subsystem);
	}
};

#endif//STRAITX_SUBSYSTEM_HPP