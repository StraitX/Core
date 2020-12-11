#ifndef STRAITX_SCREEN_WIN32_HPP
#define STRAITX_SCREEN_WIN32_HPP 

#include "platform/types.hpp"

namespace StraitX {
namespace Windows {

class ScreenWin32 {
private:
	void* m_Handle;
	bool m_IsPrimary;
	Size2i m_Size;
    Size2f m_DPI;
public:
	ScreenWin32(void *handle, bool primary, const Size2i &size, const Size2f &dpi);

	const Size2i &Size()const;

	const Size2f &DPI()const;
};

};// namespace Windows::
}; // namespace StraitX::

#endif //STRAITX_SCREEN_WIN32_HPP 