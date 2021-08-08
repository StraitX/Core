#ifndef STRAITX_SCREEN_HPP
#define STRAITX_SCREEN_HPP

#include "core/env/os.hpp"
#include "core/types.hpp"

struct PlatformScreen{
	void *Handle = nullptr;
	Size2f DPI = {};
	Size2i Size = {};
};

#endif // STRAITX_SCREEN_HPP