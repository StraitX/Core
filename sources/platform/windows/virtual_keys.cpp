#include "core/os/windows/virtual_keys.hpp"
#include <windows.h>

namespace Windows {

short MouseButtonToVirtualKey(Mouse::Button button) {
	switch (button)
	{
    case Mouse::Unknown:   return 0;
    case Mouse::Left:      return VK_LBUTTON;
    case Mouse::Right:     return VK_RBUTTON;
    case Mouse::Middle:    return VK_MBUTTON;
    case Mouse::XButton1:  return VK_XBUTTON1;
    case Mouse::XButton2:  return VK_XBUTTON2;
    default: return 0;
	}
}

Mouse::Button VirtualKeyToMouseButton(short button) {
	switch (button)
	{
    case 0:             return Mouse::Unknown;
    case VK_LBUTTON:    return Mouse::Left;
    case VK_RBUTTON:    return Mouse::Right;
    case VK_MBUTTON:    return Mouse::Middle;
    case VK_XBUTTON1:   return Mouse::XButton1;
    case VK_XBUTTON2:   return Mouse::XButton2;
    default:            return Mouse::Unknown;
	}
}

short KeyCodeToVirtualKey(Key code) {
	switch (code)
	{
    case Key::A: return 65;
    case Key::B: return 66;
    case Key::C: return 67;
    case Key::D: return 68;
    case Key::E: return 69;
    case Key::F: return 70;
    case Key::G: return 71;
    case Key::H: return 72;
    case Key::I: return 73;
    case Key::J: return 74;
    case Key::K: return 75;
    case Key::L: return 76;
    case Key::M: return 77;
    case Key::N: return 78;
    case Key::O: return 79;
    case Key::P: return 80;
    case Key::Q: return 81;
    case Key::R: return 82;
    case Key::S: return 83;
    case Key::T: return 84;
    case Key::U: return 85;
    case Key::V: return 86;
    case Key::W: return 87;
    case Key::X: return 88;
    case Key::Y: return 89;
    case Key::Z: return 90;
    case Key::Key_0: return 48;
    case Key::Key_1: return 49;
    case Key::Key_2: return 50;
    case Key::Key_3: return 51;
    case Key::Key_4: return 52;
    case Key::Key_5: return 53;
    case Key::Key_6: return 54;
    case Key::Key_7: return 55;
    case Key::Key_8: return 56;
    case Key::Key_9: return 57;
    case Key::Unknown: return 0;
    case Key::LeftBracket: return VK_OEM_4;
    case Key::Backslash: return VK_OEM_5;
    case Key::RightBracket: return VK_OEM_6;
    case Key::Space: return VK_SPACE;
    case Key::Quote: return VK_OEM_7;
    case Key::Comma: return VK_OEM_COMMA;
    case Key::Minus:  return VK_OEM_MINUS;
    case Key::Period:  return VK_OEM_PERIOD;
    case Key::Slash:  return VK_OEM_2;
    case Key::Semicolon:  return VK_OEM_1;
    case Key::Equal: return VK_OEM_PLUS;
    case Key::GraveAccent:  return VK_OEM_3;
    case Key::Escape: return VK_ESCAPE;
    case Key::Enter: return VK_RETURN;
    case Key::Tab: return VK_TAB;
    case Key::Backspace: return VK_BACK;
    case Key::Insert: return VK_INSERT;
    case Key::Delete: return VK_DELETE;
    case Key::Right: return VK_RIGHT;
    case Key::Left: return VK_LEFT;
    case Key::Down: return VK_DOWN;
    case Key::Up: return VK_UP;
    case Key::PageUp: return VK_PRIOR;
    case Key::PageDown: return VK_NEXT;
    case Key::Home: return VK_HOME;
    case Key::End: return VK_END;
    case Key::CapsLock: return VK_CAPITAL;
    case Key::ScrollLock: return VK_SCROLL;
    case Key::NumLock: return VK_NUMLOCK;
    case Key::PrintScreen: return VK_PRINT;
    case Key::Pause: return VK_PAUSE;
    case Key::F1: return VK_F1;
    case Key::F2: return VK_F2;
    case Key::F3: return VK_F3;
    case Key::F4: return VK_F4;
    case Key::F5: return VK_F5;
    case Key::F6: return VK_F6;
    case Key::F7: return VK_F7;
    case Key::F8: return VK_F8;
    case Key::F9: return VK_F9;
    case Key::F10: return VK_F10;
    case Key::F11: return VK_F11;
    case Key::F12: return VK_F12;
    case Key::F13: return VK_F13;
    case Key::F14: return VK_F14;
    case Key::F15: return VK_F15;
    case Key::F16: return VK_F16;
    case Key::F17: return VK_F17;
    case Key::F18: return VK_F18;
    case Key::F19: return VK_F19;
    case Key::F20: return VK_F20;
    case Key::F21: return VK_F21;
    case Key::F22: return VK_F22;
    case Key::F23: return VK_F23;
    case Key::F24: return VK_F24;
    case Key::Keypad_0: return VK_NUMPAD0;
    case Key::Keypad_1: return VK_NUMPAD1;
    case Key::Keypad_2: return VK_NUMPAD2;
    case Key::Keypad_3: return VK_NUMPAD3;
    case Key::Keypad_4: return VK_NUMPAD4;
    case Key::Keypad_5: return VK_NUMPAD5;
    case Key::Keypad_6: return VK_NUMPAD6;
    case Key::Keypad_7: return VK_NUMPAD7;
    case Key::Keypad_8: return VK_NUMPAD8;
    case Key::Keypad_9: return VK_NUMPAD9;
    case Key::KeypadDecimal: return VK_DECIMAL;
    case Key::KeypadDivide: return VK_DIVIDE;
    case Key::KeypadMultiply: return VK_MULTIPLY;
    case Key::KeypadSubstract: return VK_SUBTRACT;
    case Key::KeypadAdd: return VK_ADD;
    case Key::KeypadEnter: return 0; // is not supported
    case Key::KeypadEqual: return 0; // is not supported
    case Key::LeftShift: return VK_LSHIFT;
    case Key::LeftControl: return VK_LCONTROL;
    case Key::LeftAlt: return VK_LMENU;
    case Key::LeftSuper: return VK_LWIN;
    case Key::RightShift: return VK_RSHIFT;
    case Key::RightControl: return VK_RCONTROL;
    case Key::RightAlt: return VK_RMENU;
    case Key::RightSuper: return VK_RWIN;
    case Key::Menu: return VK_APPS;
    default: return 0;
	}
}

Key VirtualKeyToKeyCode(short code) {
    switch (code)
    {
    case 0: return Key::Unknown;
    case 65: return Key::A;
    case 66: return Key::B;
    case 67: return Key::C;
    case 68: return Key::D;
    case 69: return Key::E;
    case 70: return Key::F;
    case 71: return Key::G;
    case 72: return Key::H;
    case 73: return Key::I;
    case 74: return Key::J;
    case 75: return Key::K;
    case 76: return Key::L;
    case 77: return Key::M;
    case 78: return Key::N;
    case 79: return Key::O;
    case 80: return Key::P;
    case 81: return Key::Q;
    case 82: return Key::R;
    case 83: return Key::S;
    case 84: return Key::T;
    case 85: return Key::U;
    case 86: return Key::V;
    case 87: return Key::W;
    case 88: return Key::X;
    case 89: return Key::Y;
    case 90: return Key::Z;
    case 48: return Key::Key_0;
    case 49: return Key::Key_1;
    case 50: return Key::Key_2;
    case 51: return Key::Key_3;
    case 52: return Key::Key_4;
    case 53: return Key::Key_5;
    case 54: return Key::Key_6;
    case 55: return Key::Key_7;
    case 56: return Key::Key_8;
    case 57: return Key::Key_9;
    case VK_OEM_4: return Key::LeftBracket;
    case VK_OEM_5: return Key::Backslash;
    case VK_OEM_6: return Key::RightBracket;
    case VK_SPACE: return Key::Space;
    case VK_OEM_7: return Key::Quote;
    case VK_OEM_COMMA: return Key::Comma;
    case VK_OEM_MINUS:  return Key::Minus;
    case VK_OEM_PERIOD:  return Key::Period;
    case VK_OEM_2:  return Key::Slash;
    case VK_OEM_1:  return Key::Semicolon;
    case VK_OEM_PLUS: return Key::Equal;
    case VK_OEM_3:  return Key::GraveAccent;
    case VK_ESCAPE: return Key::Escape;
    case VK_RETURN: return Key::Enter;
    case VK_TAB: return Key::Tab;
    case VK_BACK: return Key::Backspace;
    case VK_INSERT: return Key::Insert;
    case VK_DELETE: return Key::Delete;
    case VK_RIGHT: return Key::Right;
    case VK_LEFT: return Key::Left;
    case VK_DOWN: return Key::Down;
    case VK_UP: return Key::Up;
    case VK_PRIOR: return Key::PageUp;
    case VK_NEXT: return Key::PageDown;
    case VK_HOME: return Key::Home;
    case VK_END: return Key::End;
    case VK_CAPITAL: return Key::CapsLock;
    case VK_SCROLL: return Key::ScrollLock;
    case VK_NUMLOCK: return Key::NumLock;
    case VK_PRINT: return Key::PrintScreen;
    case VK_PAUSE: return Key::Pause;
    case VK_F1: return Key::F1;
    case VK_F2: return Key::F2;
    case VK_F3: return Key::F3;
    case VK_F4: return Key::F4;
    case VK_F5: return Key::F5;
    case VK_F6: return Key::F6;
    case VK_F7: return Key::F7;
    case VK_F8: return Key::F8;
    case VK_F9: return Key::F9;
    case VK_F10: return Key::F10;
    case VK_F11: return Key::F11;
    case VK_F12: return Key::F12;
    case VK_F13: return Key::F13;
    case VK_F14: return Key::F14;
    case VK_F15: return Key::F15;
    case VK_F16: return Key::F16;
    case VK_F17: return Key::F17;
    case VK_F18: return Key::F18;
    case VK_F19: return Key::F19;
    case VK_F20: return Key::F20;
    case VK_F21: return Key::F21;
    case VK_F22: return Key::F22;
    case VK_F23: return Key::F23;
    case VK_F24: return Key::F24;
    case VK_NUMPAD0: return Key::Keypad_0;
    case VK_NUMPAD1: return Key::Keypad_1;
    case VK_NUMPAD2: return Key::Keypad_2;
    case VK_NUMPAD3: return Key::Keypad_3;
    case VK_NUMPAD4: return Key::Keypad_4;
    case VK_NUMPAD5: return Key::Keypad_5;
    case VK_NUMPAD6: return Key::Keypad_6;
    case VK_NUMPAD7: return Key::Keypad_7;
    case VK_NUMPAD8: return Key::Keypad_8;
    case VK_NUMPAD9: return Key::Keypad_9;
    case VK_DECIMAL: return Key::KeypadDecimal;
    case VK_DIVIDE: return Key::KeypadDivide;
    case VK_MULTIPLY: return Key::KeypadMultiply;
    case VK_SUBTRACT: return Key::KeypadSubstract;
    case VK_ADD: return Key::KeypadAdd;
    case VK_LSHIFT: return Key::LeftShift;
    case VK_LCONTROL: return Key::LeftControl;
    case VK_LMENU: return Key::LeftAlt;
    case VK_LWIN: return Key::LeftSuper;
    case VK_RSHIFT: return Key::RightShift;
    case VK_RCONTROL: return Key::RightControl;
    case VK_RMENU: return Key::RightAlt;
    case VK_RWIN: return Key::RightSuper;
    case VK_APPS: return Key::Menu;
    default: return Key::Unknown;
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

}//namespace Windows::