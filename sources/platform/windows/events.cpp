#include "platform/windows/events.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"

namespace Windows{

bool ToStraitXEvent(MSG& message, Event& event) {
	switch (message.message)
	{
	case WM_CLOSE:
		event.Type = EventType::WindowClose;
		return true;

    case WM_SIZE:
		event.Type = EventType::WindowResized;
		event.WindowResized.x = LOWORD(message.lParam);
		event.WindowResized.y = HIWORD(message.lParam);
		return true;
	case WM_PAINT:
		event.Type = EventType::WindowDraw;
		return true;
	case WM_MOUSEWHEEL:
		event.Type = EventType::MouseWheel;
		event.MouseWheel.Delta = GET_WHEEL_DELTA_WPARAM(message.wParam) / WHEEL_DELTA;
		return true;

	case WM_LBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Left;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_MBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Middle;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_RBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = Mouse::Right;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_XBUTTONDOWN:
		event.Type = EventType::MouseButtonPress;
		event.MouseButtonPress.Button = HIWORD(message.wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
		event.MouseButtonPress.x = LOWORD(message.lParam);
		event.MouseButtonPress.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_LBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Left;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_RBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Right;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_MBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = Mouse::Middle;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_XBUTTONUP:
		event.Type = EventType::MouseButtonRelease;
		event.MouseButtonRelease.Button = HIWORD(message.wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
		event.MouseButtonRelease.x = LOWORD(message.lParam);
		event.MouseButtonRelease.y = WindowImpl::GetSizeFromHandle(message.hwnd).height - HIWORD(message.lParam);
		return true;

	case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
		event.Type = EventType::KeyPress;
		event.KeyPress.KeyCode = VirtualKeyExtendedToKeyCode(message.wParam,message.lParam);
		return true;

	case WM_KEYUP:
    case WM_SYSKEYUP:
		event.Type = EventType::KeyRelease;
		event.KeyRelease.KeyCode = VirtualKeyExtendedToKeyCode(message.wParam,message.lParam);
		return true;
	case WM_CHAR:
		// TODO handle other utf characters
		if (message.wParam < 127) {
			event.Type = EventType::TextEntered;
			event.TextEntered.Unicode = message.wParam;
			return true;
		}
		return false;
	case WM_SETFOCUS:
		event.Type = EventType::FocusIn;
		return true;
	case WM_KILLFOCUS:
		event.Type = EventType::FocusOut;
		return true;
	default:
		// we don't care
		return false;
	}
}

}//namespace Windows::