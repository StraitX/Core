#ifndef STRAITX_WINDOWS_SCREEN_HPP
#define STRAITX_WINDOWS_SCREEN_HPP

#include "platform/types.hpp"

namespace Windows {

struct ScreenImpl {
	void* Handle = nullptr;
	bool IsPrimary = true;
	Size2i Size = {};
    Size2f DPI = {};

	ScreenImpl(Size2i size);

};

}//namespace Windows::

#endif //STRAITX_WINDOWS_SCREEN_HPP