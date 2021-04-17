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

short KeyCodeToVirtualKey(Key code) {
	switch (code)
	{
    case StraitX::Key::A: return 65;
    case StraitX::Key::B: return 66;
    case StraitX::Key::C: return 67;
    case StraitX::Key::D: return 68;
    case StraitX::Key::E: return 69;
    case StraitX::Key::F: return 70;
    case StraitX::Key::G: return 71;
    case StraitX::Key::H: return 72;
    case StraitX::Key::I: return 73;
    case StraitX::Key::J: return 74;
    case StraitX::Key::K: return 75;
    case StraitX::Key::L: return 76;
    case StraitX::Key::M: return 77;
    case StraitX::Key::N: return 78;
    case StraitX::Key::O: return 79;
    case StraitX::Key::P: return 80;
    case StraitX::Key::Q: return 81;
    case StraitX::Key::R: return 82;
    case StraitX::Key::S: return 83;
    case StraitX::Key::T: return 84;
    case StraitX::Key::U: return 85;
    case StraitX::Key::V: return 86;
    case StraitX::Key::W: return 87;
    case StraitX::Key::X: return 88;
    case StraitX::Key::Y: return 89;
    case StraitX::Key::Z: return 90;
    case StraitX::Key::Key_0: return 48;
    case StraitX::Key::Key_1: return 49;
    case StraitX::Key::Key_2: return 50;
    case StraitX::Key::Key_3: return 51;
    case StraitX::Key::Key_4: return 52;
    case StraitX::Key::Key_5: return 53;
    case StraitX::Key::Key_6: return 54;
    case StraitX::Key::Key_7: return 55;
    case StraitX::Key::Key_8: return 56;
    case StraitX::Key::Key_9: return 57;
    case StraitX::Key::Unknown: return 0;
    case StraitX::Key::LeftBracket: return VK_OEM_4;
    case StraitX::Key::Backslash: return VK_OEM_5;
    case StraitX::Key::RightBracket: return VK_OEM_6;
    case StraitX::Key::Space: return VK_SPACE;
    case StraitX::Key::Quote: return VK_OEM_7;
    case StraitX::Key::Comma: return VK_OEM_COMMA;
    case StraitX::Key::Minus:  return VK_OEM_MINUS;
    case StraitX::Key::Period:  return VK_OEM_PERIOD;
    case StraitX::Key::Slash:  return VK_OEM_2;
    case StraitX::Key::Semicolon:  return VK_OEM_1;
    case StraitX::Key::Equal: return VK_OEM_PLUS;
    case StraitX::Key::GraveAccent:  return VK_OEM_3;
    case StraitX::Key::Escape: return VK_ESCAPE;
    case StraitX::Key::Enter: return VK_RETURN;
    case StraitX::Key::Tab: return VK_TAB;
    case StraitX::Key::Backspace: return VK_BACK;
    case StraitX::Key::Insert: return VK_INSERT;
    case StraitX::Key::Delete: return VK_DELETE;
    case StraitX::Key::Right: return VK_RIGHT;
    case StraitX::Key::Left: return VK_LEFT;
    case StraitX::Key::Down: return VK_DOWN;
    case StraitX::Key::Up: return VK_UP;
    case StraitX::Key::PageUp: return VK_PRIOR;
    case StraitX::Key::PageDown: return VK_NEXT;
    case StraitX::Key::Home: return VK_HOME;
    case StraitX::Key::End: return VK_END;
    case StraitX::Key::CapsLock: return VK_CAPITAL;
    case StraitX::Key::ScrollLock: return VK_SCROLL;
    case StraitX::Key::NumLock: return VK_NUMLOCK;
    case StraitX::Key::PrintScreen: return VK_PRINT;
    case StraitX::Key::Pause: return VK_PAUSE;
    case StraitX::Key::F1: return VK_F1;
    case StraitX::Key::F2: return VK_F2;
    case StraitX::Key::F3: return VK_F3;
    case StraitX::Key::F4: return VK_F4;
    case StraitX::Key::F5: return VK_F5;
    case StraitX::Key::F6: return VK_F6;
    case StraitX::Key::F7: return VK_F7;
    case StraitX::Key::F8: return VK_F8;
    case StraitX::Key::F9: return VK_F9;
    case StraitX::Key::F10: return VK_F10;
    case StraitX::Key::F11: return VK_F11;
    case StraitX::Key::F12: return VK_F12;
    case StraitX::Key::F13: return VK_F13;
    case StraitX::Key::F14: return VK_F14;
    case StraitX::Key::F15: return VK_F15;
    case StraitX::Key::F16: return VK_F16;
    case StraitX::Key::F17: return VK_F17;
    case StraitX::Key::F18: return VK_F18;
    case StraitX::Key::F19: return VK_F19;
    case StraitX::Key::F20: return VK_F20;
    case StraitX::Key::F21: return VK_F21;
    case StraitX::Key::F22: return VK_F22;
    case StraitX::Key::F23: return VK_F23;
    case StraitX::Key::F24: return VK_F24;
    case StraitX::Key::Keypad_0: return VK_NUMPAD0;
    case StraitX::Key::Keypad_1: return VK_NUMPAD1;
    case StraitX::Key::Keypad_2: return VK_NUMPAD2;
    case StraitX::Key::Keypad_3: return VK_NUMPAD3;
    case StraitX::Key::Keypad_4: return VK_NUMPAD4;
    case StraitX::Key::Keypad_5: return VK_NUMPAD5;
    case StraitX::Key::Keypad_6: return VK_NUMPAD6;
    case StraitX::Key::Keypad_7: return VK_NUMPAD7;
    case StraitX::Key::Keypad_8: return VK_NUMPAD8;
    case StraitX::Key::Keypad_9: return VK_NUMPAD9;
    case StraitX::Key::KeypadDecimal: return VK_DECIMAL;
    case StraitX::Key::KeypadDivide: return VK_DIVIDE;
    case StraitX::Key::KeypadMultiply: return VK_MULTIPLY;
    case StraitX::Key::KeypadSubstract: return VK_SUBTRACT;
    case StraitX::Key::KeypadAdd: return VK_ADD;
    case StraitX::Key::KeypadEnter: return 0; // is not supported
    case StraitX::Key::KeypadEqual: return 0; // is not supported
    case StraitX::Key::LeftShift: return VK_LSHIFT;
    case StraitX::Key::LeftControl: return VK_LCONTROL;
    case StraitX::Key::LeftAlt: return VK_LMENU;
    case StraitX::Key::LeftSuper: return VK_LWIN;
    case StraitX::Key::RightShift: return VK_RSHIFT;
    case StraitX::Key::RightControl: return VK_RCONTROL;
    case StraitX::Key::RightAlt: return VK_RMENU;
    case StraitX::Key::RightSuper: return VK_RWIN;
    case StraitX::Key::Menu: return VK_APPS;
    default: return 0;
	}
}

Key VirtualKeyToKeyCode(short code) {
    switch (code)
    {
    case 0: return StraitX::Key::Unknown;
    case 65: return StraitX::Key::A;
    case 66: return StraitX::Key::B;
    case 67: return StraitX::Key::C;
    case 68: return StraitX::Key::D;
    case 69: return StraitX::Key::E;
    case 70: return StraitX::Key::F;
    case 71: return StraitX::Key::G;
    case 72: return StraitX::Key::H;
    case 73: return StraitX::Key::I;
    case 74: return StraitX::Key::J;
    case 75: return StraitX::Key::K;
    case 76: return StraitX::Key::L;
    case 77: return StraitX::Key::M;
    case 78: return StraitX::Key::N;
    case 79: return StraitX::Key::O;
    case 80: return StraitX::Key::P;
    case 81: return StraitX::Key::Q;
    case 82: return StraitX::Key::R;
    case 83: return StraitX::Key::S;
    case 84: return StraitX::Key::T;
    case 85: return StraitX::Key::U;
    case 86: return StraitX::Key::V;
    case 87: return StraitX::Key::W;
    case 88: return StraitX::Key::X;
    case 89: return StraitX::Key::Y;
    case 90: return StraitX::Key::Z;
    case 48: return StraitX::Key::Key_0;
    case 49: return StraitX::Key::Key_1;
    case 50: return StraitX::Key::Key_2;
    case 51: return StraitX::Key::Key_3;
    case 52: return StraitX::Key::Key_4;
    case 53: return StraitX::Key::Key_5;
    case 54: return StraitX::Key::Key_6;
    case 55: return StraitX::Key::Key_7;
    case 56: return StraitX::Key::Key_8;
    case 57: return StraitX::Key::Key_9;
    case VK_OEM_4: return StraitX::Key::LeftBracket;
    case VK_OEM_5: return StraitX::Key::Backslash;
    case VK_OEM_6: return StraitX::Key::RightBracket;
    case VK_SPACE: return StraitX::Key::Space;
    case VK_OEM_7: return StraitX::Key::Quote;
    case VK_OEM_COMMA: return StraitX::Key::Comma;
    case VK_OEM_MINUS:  return StraitX::Key::Minus;
    case VK_OEM_PERIOD:  return StraitX::Key::Period;
    case VK_OEM_2:  return StraitX::Key::Slash;
    case VK_OEM_1:  return StraitX::Key::Semicolon;
    case VK_OEM_PLUS: return StraitX::Key::Equal;
    case VK_OEM_3:  return StraitX::Key::GraveAccent;
    case VK_ESCAPE: return StraitX::Key::Escape;
    case VK_RETURN: return StraitX::Key::Enter;
    case VK_TAB: return StraitX::Key::Tab;
    case VK_BACK: return StraitX::Key::Backspace;
    case VK_INSERT: return StraitX::Key::Insert;
    case VK_DELETE: return StraitX::Key::Delete;
    case VK_RIGHT: return StraitX::Key::Right;
    case VK_LEFT: return StraitX::Key::Left;
    case VK_DOWN: return StraitX::Key::Down;
    case VK_UP: return StraitX::Key::Up;
    case VK_PRIOR: return StraitX::Key::PageUp;
    case VK_NEXT: return StraitX::Key::PageDown;
    case VK_HOME: return StraitX::Key::Home;
    case VK_END: return StraitX::Key::End;
    case VK_CAPITAL: return StraitX::Key::CapsLock;
    case VK_SCROLL: return StraitX::Key::ScrollLock;
    case VK_NUMLOCK: return StraitX::Key::NumLock;
    case VK_PRINT: return StraitX::Key::PrintScreen;
    case VK_PAUSE: return StraitX::Key::Pause;
    case VK_F1: return StraitX::Key::F1;
    case VK_F2: return StraitX::Key::F2;
    case VK_F3: return StraitX::Key::F3;
    case VK_F4: return StraitX::Key::F4;
    case VK_F5: return StraitX::Key::F5;
    case VK_F6: return StraitX::Key::F6;
    case VK_F7: return StraitX::Key::F7;
    case VK_F8: return StraitX::Key::F8;
    case VK_F9: return StraitX::Key::F9;
    case VK_F10: return StraitX::Key::F10;
    case VK_F11: return StraitX::Key::F11;
    case VK_F12: return StraitX::Key::F12;
    case VK_F13: return StraitX::Key::F13;
    case VK_F14: return StraitX::Key::F14;
    case VK_F15: return StraitX::Key::F15;
    case VK_F16: return StraitX::Key::F16;
    case VK_F17: return StraitX::Key::F17;
    case VK_F18: return StraitX::Key::F18;
    case VK_F19: return StraitX::Key::F19;
    case VK_F20: return StraitX::Key::F20;
    case VK_F21: return StraitX::Key::F21;
    case VK_F22: return StraitX::Key::F22;
    case VK_F23: return StraitX::Key::F23;
    case VK_F24: return StraitX::Key::F24;
    case VK_NUMPAD0: return StraitX::Key::Keypad_0;
    case VK_NUMPAD1: return StraitX::Key::Keypad_1;
    case VK_NUMPAD2: return StraitX::Key::Keypad_2;
    case VK_NUMPAD3: return StraitX::Key::Keypad_3;
    case VK_NUMPAD4: return StraitX::Key::Keypad_4;
    case VK_NUMPAD5: return StraitX::Key::Keypad_5;
    case VK_NUMPAD6: return StraitX::Key::Keypad_6;
    case VK_NUMPAD7: return StraitX::Key::Keypad_7;
    case VK_NUMPAD8: return StraitX::Key::Keypad_8;
    case VK_NUMPAD9: return StraitX::Key::Keypad_9;
    case VK_DECIMAL: return StraitX::Key::KeypadDecimal;
    case VK_DIVIDE: return StraitX::Key::KeypadDivide;
    case VK_MULTIPLY: return StraitX::Key::KeypadMultiply;
    case VK_SUBTRACT: return StraitX::Key::KeypadSubstract;
    case VK_ADD: return StraitX::Key::KeypadAdd;
    case VK_LSHIFT: return StraitX::Key::LeftShift;
    case VK_LCONTROL: return StraitX::Key::LeftControl;
    case VK_LMENU: return StraitX::Key::LeftAlt;
    case VK_LWIN: return StraitX::Key::LeftSuper;
    case VK_RSHIFT: return StraitX::Key::RightShift;
    case VK_RCONTROL: return StraitX::Key::RightControl;
    case VK_RMENU: return StraitX::Key::RightAlt;
    case VK_RWIN: return StraitX::Key::RightSuper;
    case VK_APPS: return StraitX::Key::Menu;
    default: return StraitX::Key::Unknown;
    }
}

Key VirtualKeyExtendedToKeyCode(WPARAM key, LPARAM flags) {
	switch (key) {
	case VK_SHIFT:
        static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
        return static_cast<UINT>((flags & (0xFF << 16)) >> 16) == lShift ? Key::LeftShift : Key::RightShift;
        
    case VK_MENU: 
		return (HIWORD(flags) & KF_EXTENDED) ? Key::RightAlt : Key::LeftAlt;

	case VK_CONTROL: 
		return (HIWORD(flags) & KF_EXTENDED) ? Key::RightControl : Key::LeftControl;

	default:
		return VirtualKeyToKeyCode(key);
	}
}

}; // namespace Windows::
}; // namespace StraitX::