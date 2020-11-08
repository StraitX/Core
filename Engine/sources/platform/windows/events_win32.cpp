#include "platform/windows/events_win32.hpp"
#include "platform/windows/virtual_keys.hpp"

namespace StraitX{
namespace Windows{

bool ToStraitXEvent(MSG& message, Event& event) {
	switch (message.message)
	{
	case WM_SX_CLOSE:
		event.Type = EventType::WindowClose;
		return true;

	case WM_PAINT:
		event.Type = EventType::WindowDraw;
		return true;

	case WM_LBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Left;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = HIWORD(message.lParam);
		return true;

	case WM_MBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Middle;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = HIWORD(message.lParam);
		return true;

	case WM_RBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Right;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = HIWORD(message.lParam);
		return true;

	case WM_XBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = HIWORD(message.wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = HIWORD(message.lParam);
		return true;

	case WM_LBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Left;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = HIWORD(message.lParam);
		return true;

	case WM_RBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Right;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = HIWORD(message.lParam);
		return true;

	case WM_MBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Middle;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = HIWORD(message.lParam);
		return true;

	case WM_XBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = HIWORD(message.wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = HIWORD(message.lParam);
		return true;

	case WM_SX_KEYDOWN:
		event.Type = EventType::KeyPress;
		event.KeyPress.KeyCode = VirtualKeyExtendedToKeyCode(message.wParam,message.lParam);
		return true;

	case WM_SX_KEYUP:
		event.Type = EventType::KeyRelease;
		event.KeyRelease.KeyCode = VirtualKeyExtendedToKeyCode(message.wParam,message.lParam);
		return true;

	default:
		// we don't care
		return false;
	}
}

}; //namespace Windows::
}; //namespace StraitX::