#ifndef STRAITX_ENTRY_POINT_HPP
#define STRAITX_ENTRY_POINT_HPP

#include "core/result.hpp"
#include "core/os/events.hpp"

struct PlatformRuntime{

	static Result Initialize();

	static bool Tick(float dt);

	static void Finalize();

};

#endif//STRAITX_ENTRY_POINT_HPP