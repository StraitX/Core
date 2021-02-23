#ifndef STRAITX_WINDOWS_SCREEN_HPP
#define STRAITX_WINDOWS_SCREEN_HPP

#include "platform/types.hpp"

namespace StraitX {
namespace Windows {

class ScreenImpl {
private:
	void* m_Handle;
	bool m_IsPrimary;
	Size2i m_Size;
    Size2f m_DPI;
public:
	ScreenImpl(void *handle, bool primary, const Size2i &size, const Size2f &dpi);

	const Size2i &Size()const;

	const Size2f &DPI()const;
};

};// namespace Windows::
}; // namespace StraitX::

#endif //STRAITX_WINDOWS_SCREEN_HPP