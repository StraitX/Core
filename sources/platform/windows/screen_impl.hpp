#ifndef STRAITX_WINDOWS_SCREEN_HPP
#define STRAITX_WINDOWS_SCREEN_HPP

#include "core/types.hpp"

namespace Windows {

struct ScreenImpl {
	void* Handle = nullptr;
	bool IsPrimary = true;
	Vector2i Size = {};
    Vector2f DPI = {};
};

}//namespace Windows::

#endif //STRAITX_WINDOWS_SCREEN_HPP