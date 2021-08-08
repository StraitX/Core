#ifndef STRAITX_SCREEN_HPP
#define STRAITX_SCREEN_HPP

#include "core/env/os.hpp"
#include "core/types.hpp"
#include "core/math/vector2.hpp"

struct PlatformScreen{
	void *Handle = nullptr;
	Vector2f DPI = {};
	Vector2s Size = {};
};

#endif // STRAITX_SCREEN_HPP