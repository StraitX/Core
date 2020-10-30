#include "platform/windows/virtual_keys.hpp"
#include <windows.h>

namespace StraitX {
namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button) {
	switch (button)
	{
    case StraitX::Mouse::Left:      return VK_LBUTTON;
    case StraitX::Mouse::Right:     return VK_RBUTTON;
    case StraitX::Mouse::Middle:    return VK_MBUTTON;
    case StraitX::Mouse::XButton1:  return VK_XBUTTON1;
    case StraitX::Mouse::XButton2:  return VK_XBUTTON2;
    default: return 0;
	}
}


}; // namespace Windows::
}; // namespace StraitX::