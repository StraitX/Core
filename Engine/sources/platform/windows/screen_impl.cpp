#include "platform/windows/screen_impl.hpp"

namespace StraitX {
namespace Windows {

ScreenImpl::ScreenImpl(void* handle, bool primary, const Size2i& size, const Size2f& dpi):
	m_Handle(handle), m_IsPrimary(primary), m_Size(size), m_DPI(dpi)
{}

const Size2i& ScreenImpl::Size()const {
	return m_Size;
}

const Size2f& ScreenImpl::DPI()const {
	return m_DPI;
}

};// namespace Windows::
}; // namespace StraitX::