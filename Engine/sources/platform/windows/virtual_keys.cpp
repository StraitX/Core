#include "platform/windows/virtual_keys.hpp"
#include <windows.h>

namespace StraitX {
namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button) {
	switch (button)
	{
    case StraitX::Mouse::Unknown:   return 0;
    case StraitX::Mouse::Left:      return VK_LBUTTON;
    case StraitX::Mouse::Right:     return VK_RBUTTON;
    case StraitX::Mouse::Middle:    return VK_MBUTTON;
    case StraitX::Mouse::XButton1:  return VK_XBUTTON1;
    case StraitX::Mouse::XButton2:  return VK_XBUTTON2;
    default: return 0;
	}
}

Mouse::Button VirtualKeyToMouseButton(short button) {
	switch (button)
	{
    case 0:             return StraitX::Mouse::Unknown;
    case VK_LBUTTON:    return StraitX::Mouse::Left;
    case VK_RBUTTON:    return StraitX::Mouse::Right;
    case VK_MBUTTON:    return StraitX::Mouse::Middle;
    case VK_XBUTTON1:   return StraitX::Mouse::XButton1;
    case VK_XBUTTON2:   return StraitX::Mouse::XButton2;
    default:            return StraitX::Mouse::Unknown;
	}
}

short KeyCodeToVirtualKey(Keyboard::KeyCode code) {
    if (code >= 65 && code <= 90 || code >= 48 && code <= 57)
        return (short)code;
	switch (code)
	{
    case StraitX::Keyboard::Unknown: return 0;
    case StraitX::Keyboard::LeftBracket: return VK_OEM_4;
    case StraitX::Keyboard::Backslash: return VK_OEM_5;
    case StraitX::Keyboard::RightBracket: return VK_OEM_6;
    case StraitX::Keyboard::Space: return VK_SPACE;
    case StraitX::Keyboard::Quote: return VK_OEM_7;
    case StraitX::Keyboard::Comma: return VK_OEM_COMMA;
    case StraitX::Keyboard::Minus:  return VK_OEM_MINUS;
    case StraitX::Keyboard::Period:  return VK_OEM_PERIOD;
    case StraitX::Keyboard::Slash:  return VK_OEM_2;
    case StraitX::Keyboard::Semicolon:  return VK_OEM_1;
    case StraitX::Keyboard::Equal: return VK_OEM_PLUS;
    case StraitX::Keyboard::GraveAccent:  return VK_OEM_3;
    case StraitX::Keyboard::Escape: return VK_ESCAPE;
    case StraitX::Keyboard::Enter: return VK_RETURN;
    case StraitX::Keyboard::Tab: return VK_TAB;
    case StraitX::Keyboard::Backspace: return VK_BACK;
    case StraitX::Keyboard::Insert: return VK_INSERT;
    case StraitX::Keyboard::Delete: return VK_DELETE;
    case StraitX::Keyboard::Right: return VK_RIGHT;
    case StraitX::Keyboard::Left: return VK_LEFT;
    case StraitX::Keyboard::Down: return VK_DOWN;
    case StraitX::Keyboard::Up: return VK_UP;
    case StraitX::Keyboard::PageUp: return VK_PRIOR;
    case StraitX::Keyboard::PageDown: return VK_NEXT;
    case StraitX::Keyboard::Home: return VK_HOME;
    case StraitX::Keyboard::End: return VK_END;
    case StraitX::Keyboard::CapsLock: return VK_CAPITAL;
    case StraitX::Keyboard::ScrollLock: return VK_SCROLL;
    case StraitX::Keyboard::NumLock: return VK_NUMLOCK;
    case StraitX::Keyboard::PrintScreen: return VK_PRINT;
    case StraitX::Keyboard::Pause: return VK_PAUSE;
    case StraitX::Keyboard::F1: return VK_F1;
    case StraitX::Keyboard::F2: return VK_F2;
    case StraitX::Keyboard::F3: return VK_F3;
    case StraitX::Keyboard::F4: return VK_F4;
    case StraitX::Keyboard::F5: return VK_F5;
    case StraitX::Keyboard::F6: return VK_F6;
    case StraitX::Keyboard::F7: return VK_F7;
    case StraitX::Keyboard::F8: return VK_F8;
    case StraitX::Keyboard::F9: return VK_F9;
    case StraitX::Keyboard::F10: return VK_F10;
    case StraitX::Keyboard::F11: return VK_F11;
    case StraitX::Keyboard::F12: return VK_F12;
    case StraitX::Keyboard::F13: return VK_F13;
    case StraitX::Keyboard::F14: return VK_F14;
    case StraitX::Keyboard::F15: return VK_F15;
    case StraitX::Keyboard::F16: return VK_F16;
    case StraitX::Keyboard::F17: return VK_F17;
    case StraitX::Keyboard::F18: return VK_F18;
    case StraitX::Keyboard::F19: return VK_F19;
    case StraitX::Keyboard::F20: return VK_F20;
    case StraitX::Keyboard::F21: return VK_F21;
    case StraitX::Keyboard::F22: return VK_F22;
    case StraitX::Keyboard::F23: return VK_F23;
    case StraitX::Keyboard::F24: return VK_F24;
    case StraitX::Keyboard::Keypad_0: return VK_NUMPAD0;
    case StraitX::Keyboard::Keypad_1: return VK_NUMPAD1;
    case StraitX::Keyboard::Keypad_2: return VK_NUMPAD2;
    case StraitX::Keyboard::Keypad_3: return VK_NUMPAD3;
    case StraitX::Keyboard::Keypad_4: return VK_NUMPAD4;
    case StraitX::Keyboard::Keypad_5: return VK_NUMPAD5;
    case StraitX::Keyboard::Keypad_6: return VK_NUMPAD6;
    case StraitX::Keyboard::Keypad_7: return VK_NUMPAD7;
    case StraitX::Keyboard::Keypad_8: return VK_NUMPAD8;
    case StraitX::Keyboard::Keypad_9: return VK_NUMPAD9;
    case StraitX::Keyboard::KeypadDecimal: return VK_DECIMAL;
    case StraitX::Keyboard::KeypadDivide: return VK_DIVIDE;
    case StraitX::Keyboard::KeypadMultiply: return VK_MULTIPLY;
    case StraitX::Keyboard::KeypadSubstract: return VK_SUBTRACT;
    case StraitX::Keyboard::KeypadAdd: return VK_ADD;
    case StraitX::Keyboard::KeypadEnter: return 0; // is not supported
    case StraitX::Keyboard::KeypadEqual: return 0; // is not supported
    case StraitX::Keyboard::LeftShift: return VK_LSHIFT;
    case StraitX::Keyboard::LeftControl: return VK_LCONTROL;
    case StraitX::Keyboard::LeftAlt: return VK_LMENU;
    case StraitX::Keyboard::LeftSuper: return VK_LWIN;
    case StraitX::Keyboard::RightShift: return VK_RSHIFT;
    case StraitX::Keyboard::RightControl: return VK_RCONTROL;
    case StraitX::Keyboard::RightAlt: return VK_RMENU;
    case StraitX::Keyboard::RightSuper: return VK_RWIN;
    case StraitX::Keyboard::Menu: return VK_APPS;
    default: return 0;
	}
}

Keyboard::KeyCode VirtualKeyToKeyCode(short code) {
    if (code >= 65 && code <= 90 || code >= 48 && code <= 57)
        return (Keyboard::KeyCode)code;
    switch (code)
    {
    case 0: return StraitX::Keyboard::Unknown;
    case VK_OEM_4: return StraitX::Keyboard::LeftBracket;
    case VK_OEM_5: return StraitX::Keyboard::Backslash;
    case VK_OEM_6: return StraitX::Keyboard::RightBracket;
    case VK_SPACE: return StraitX::Keyboard::Space;
    case VK_OEM_7: return StraitX::Keyboard::Quote;
    case VK_OEM_COMMA: return StraitX::Keyboard::Comma;
    case VK_OEM_MINUS:  return StraitX::Keyboard::Minus;
    case VK_OEM_PERIOD:  return StraitX::Keyboard::Period;
    case VK_OEM_2:  return StraitX::Keyboard::Slash;
    case VK_OEM_1:  return StraitX::Keyboard::Semicolon;
    case VK_OEM_PLUS: return StraitX::Keyboard::Equal;
    case VK_OEM_3:  return StraitX::Keyboard::GraveAccent;
    case VK_ESCAPE: return StraitX::Keyboard::Escape;
    case VK_RETURN: return StraitX::Keyboard::Enter;
    case VK_TAB: return StraitX::Keyboard::Tab;
    case VK_BACK: return StraitX::Keyboard::Backspace;
    case VK_INSERT: return StraitX::Keyboard::Insert;
    case VK_DELETE: return StraitX::Keyboard::Delete;
    case VK_RIGHT: return StraitX::Keyboard::Right;
    case VK_LEFT: return StraitX::Keyboard::Left;
    case VK_DOWN: return StraitX::Keyboard::Down;
    case VK_UP: return StraitX::Keyboard::Up;
    case VK_PRIOR: return StraitX::Keyboard::PageUp;
    case VK_NEXT: return StraitX::Keyboard::PageDown;
    case VK_HOME: return StraitX::Keyboard::Home;
    case VK_END: return StraitX::Keyboard::End;
    case VK_CAPITAL: return StraitX::Keyboard::CapsLock;
    case VK_SCROLL: return StraitX::Keyboard::ScrollLock;
    case VK_NUMLOCK: return StraitX::Keyboard::NumLock;
    case VK_PRINT: return StraitX::Keyboard::PrintScreen;
    case VK_PAUSE: return StraitX::Keyboard::Pause;
    case VK_F1: return StraitX::Keyboard::F1;
    case VK_F2: return StraitX::Keyboard::F2;
    case VK_F3: return StraitX::Keyboard::F3;
    case VK_F4: return StraitX::Keyboard::F4;
    case VK_F5: return StraitX::Keyboard::F5;
    case VK_F6: return StraitX::Keyboard::F6;
    case VK_F7: return StraitX::Keyboard::F7;
    case VK_F8: return StraitX::Keyboard::F8;
    case VK_F9: return StraitX::Keyboard::F9;
    case VK_F10: return StraitX::Keyboard::F10;
    case VK_F11: return StraitX::Keyboard::F11;
    case VK_F12: return StraitX::Keyboard::F12;
    case VK_F13: return StraitX::Keyboard::F13;
    case VK_F14: return StraitX::Keyboard::F14;
    case VK_F15: return StraitX::Keyboard::F15;
    case VK_F16: return StraitX::Keyboard::F16;
    case VK_F17: return StraitX::Keyboard::F17;
    case VK_F18: return StraitX::Keyboard::F18;
    case VK_F19: return StraitX::Keyboard::F19;
    case VK_F20: return StraitX::Keyboard::F20;
    case VK_F21: return StraitX::Keyboard::F21;
    case VK_F22: return StraitX::Keyboard::F22;
    case VK_F23: return StraitX::Keyboard::F23;
    case VK_F24: return StraitX::Keyboard::F24;
    case VK_NUMPAD0: return StraitX::Keyboard::Keypad_0;
    case VK_NUMPAD1: return StraitX::Keyboard::Keypad_1;
    case VK_NUMPAD2: return StraitX::Keyboard::Keypad_2;
    case VK_NUMPAD3: return StraitX::Keyboard::Keypad_3;
    case VK_NUMPAD4: return StraitX::Keyboard::Keypad_4;
    case VK_NUMPAD5: return StraitX::Keyboard::Keypad_5;
    case VK_NUMPAD6: return StraitX::Keyboard::Keypad_6;
    case VK_NUMPAD7: return StraitX::Keyboard::Keypad_7;
    case VK_NUMPAD8: return StraitX::Keyboard::Keypad_8;
    case VK_NUMPAD9: return StraitX::Keyboard::Keypad_9;
    case VK_DECIMAL: return StraitX::Keyboard::KeypadDecimal;
    case VK_DIVIDE: return StraitX::Keyboard::KeypadDivide;
    case VK_MULTIPLY: return StraitX::Keyboard::KeypadMultiply;
    case VK_SUBTRACT: return StraitX::Keyboard::KeypadSubstract;
    case VK_ADD: return StraitX::Keyboard::KeypadAdd;
    case VK_LSHIFT: return StraitX::Keyboard::LeftShift;
    case VK_LCONTROL: return StraitX::Keyboard::LeftControl;
    case VK_LMENU: return StraitX::Keyboard::LeftAlt;
    case VK_LWIN: return StraitX::Keyboard::LeftSuper;
    case VK_RSHIFT: return StraitX::Keyboard::RightShift;
    case VK_RCONTROL: return StraitX::Keyboard::RightControl;
    case VK_RMENU: return StraitX::Keyboard::RightAlt;
    case VK_RWIN: return StraitX::Keyboard::RightSuper;
    case VK_APPS: return StraitX::Keyboard::Menu;
    default: return StraitX::Keyboard::Unknown;
    }
}

Keyboard::KeyCode VirtualKeyExtendedToKeyCode(WPARAM key, LPARAM flags) {
	switch (key) {
	case VK_SHIFT:
        static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
        return static_cast<UINT>((flags & (0xFF << 16)) >> 16) == lShift ? Keyboard::LeftShift : Keyboard::RightShift;
        
    case VK_MENU: 
		return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::RightAlt : Keyboard::LeftAlt;

	case VK_CONTROL: 
		return (HIWORD(flags) & KF_EXTENDED) ? Keyboard::RightControl : Keyboard::LeftControl;

	default:
		return VirtualKeyToKeyCode(key);
	}
}

}; // namespace Windows::
}; // namespace StraitX::