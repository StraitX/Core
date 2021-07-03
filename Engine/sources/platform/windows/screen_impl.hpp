#ifndef STRAITX_WINDOWS_SCREEN_HPP
#define STRAITX_WINDOWS_SCREEN_HPP

#include "platform/types.hpp"

namespace Windows {

class ScreenImpl {
private:
	void* m_Handle = nullptr;
	bool m_IsPrimary = true;
	Size2i m_Size = {};
    Size2f m_DPI = {};
public:
	ScreenImpl(Size2i size);

	const Size2i &Size()const;

	const Size2f &DPI()const;
};

}//namespace Windows::

#endif //STRAITX_WINDOWS_SCREEN_HPP