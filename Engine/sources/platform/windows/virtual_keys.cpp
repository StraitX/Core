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
    case StraitX::Keyboard::RightAlt: return VK_LMENU;
    case StraitX::Keyboard::RightSuper: return VK_RWIN;
    case StraitX::Keyboard::Menu: return VK_APPS;
    default: return 0;
	}
}

}; // namespace Windows::
}; // namespace StraitX::