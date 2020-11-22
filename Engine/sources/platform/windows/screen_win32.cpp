#include "platform/windows/screen_win32.hpp"

namespace StraitX {
namespace Windows {

ScreenWin32::ScreenWin32(void* handle, bool primary, const Size2i& size, const Size2f& dpi):
	m_Handle(handle), m_IsPrimary(primary), m_Size(size), m_DPI(dpi)
{}

const Size2i& ScreenWin32::Size()const {
	return m_Size;
}

const Size2f& ScreenWin32::DPI()const {
	return m_DPI;
}

};// namespace Windows::
}; // namespace StraitX::