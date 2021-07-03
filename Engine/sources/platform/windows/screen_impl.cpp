#include "platform/windows/screen_impl.hpp"

namespace StraitX {
namespace Windows {

ScreenImpl::ScreenImpl(Size2i size):
	m_Size(size)
{}

const Size2i& ScreenImpl::Size()const {
	return m_Size;
}

const Size2f& ScreenImpl::DPI()const {
	return m_DPI;
}

};// namespace Windows::
}; // namespace StraitX::