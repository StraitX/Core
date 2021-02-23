#ifndef STRAITX_WINDOWS_SCREEN_HPP
#define STRAITX_WINDOWS_SCREEN_HPP

#include "platform/types.hpp"

namespace StraitX {
namespace Windows {

class ScreenImpl {
private:
	void* m_Handle = nullptr;
	bool m_IsPrimary = true;
	Size2i m_Size = {};
    Size2f m_DPI = {};
public:

	const Size2i &Size()const;

	const Size2f &DPI()const;
};

};// namespace Windows::
}; // namespace StraitX::

#endif //STRAITX_WINDOWS_SCREEN_HPP