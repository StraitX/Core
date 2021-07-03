#ifndef STRAITX_ENTRY_POINT_HPP
#define STRAITX_ENTRY_POINT_HPP

#include "platform/result.hpp"
#include "platform/events.hpp"

struct PlatformRuntime{

	static Result Initialize();

	static bool Tick(float dt);

	static void ProcessEvent(const Event &e);

	static void Finalize();

};

#endif//STRAITX_ENTRY_POINT_HPP