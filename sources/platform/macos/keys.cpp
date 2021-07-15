#include "platform/macos/keys.hpp"


namespace MacOS{

Key ToStraitXKeyCode(int key_code){
    switch (key_code){
    case KEY_A: return Key::A;
    case KEY_S: return Key::S;
    case KEY_D: return Key::D;
    case KEY_F: return Key::F;
    case KEY_H: return Key::H;
    case KEY_G: return Key::G;
    case KEY_Z: return Key::Z;
    case KEY_X: return Key::X;
    case KEY_C: return Key::C;
    case KEY_V: return Key::V;
    case KEY_B: return Key::B;
    case KEY_Q: return Key::Q;
    case KEY_W: return Key::W;
    case KEY_E: return Key::E;
    case KEY_R: return Key::R;
    case KEY_Y: return Key::Y;
    case KEY_T: return Key::T;
    case KEY_1: return Key::Key_1;
    case KEY_2: return Key::Key_2;
    case KEY_3: return Key::Key_3;
    case KEY_4: return Key::Key_4;
    case KEY_6: return Key::Key_6;
    case KEY_5: return Key::Key_5;
    case KEY_EQUALS: return Key::Equal;
    case KEY_9: return Key::Key_9;
    case KEY_7: return Key::Key_7;
    case KEY_MINUS: return Key::Minus;
    case KEY_8: return Key::Key_8;
    case KEY_0: return Key::Key_0;
    case KEY_RIGHTBRACKET: return Key::RightBracket;
    case KEY_O: return Key::O;
    case KEY_U: return Key::U;
    case KEY_LEFTBRACKET: return Key::LeftBracket;
    case KEY_I: return Key::I;
    case KEY_P: return Key::P;
    case KEY_RETURN: return Key::Enter;
    case KEY_L: return Key::L;
    case KEY_J: return Key::J;
    case KEY_APOSTROPHE: return Key::Quote;
    case KEY_K: return Key::K;
    case KEY_SEMICOLON: return Key::Semicolon;
    case KEY_FRONTSLASH: return Key::Slash;
    case KEY_COMMA: return Key::Comma;
    case KEY_BACKSLASH: return Key::Backslash;
    case KEY_N: return Key::N;
    case KEY_M: return Key::M;
    case KEY_PERIOD: return Key::Period;
    case KEY_TAB: return Key::Tab;
    case KEY_SPACE: return Key::Space;
    case KEY_BACKAPOSTROPHE: return Key::GraveAccent;
    case KEY_DELETE_BACKSPACE: return Key::Backspace;
    case KEY_DELETE: return Key::Delete;
    case KEY_ESCAPE: return Key::Escape;
    case KEY_RIGHTCOMMAND: return Key::RightSuper;
    case KEY_LEFTCOMMAND: return Key::LeftSuper;
    case KEY_LEFTSHIFT: return Key::LeftShift;
    case KEY_CAPSLOCK: return Key::CapsLock;
    case KEY_LEFTALT: return Key::LeftAlt;
    case KEY_LEFTCONTROL: return Key::LeftControl;
    case KEY_RIGHTSHIFHT: return Key::RightShift;
    case KEY_RIGHTALT: return Key::RightAlt;
    case KEY_RIGHTCTRL: return Key::RightControl;
    case KEY_NUMPAD_DEL: return Key::KeypadDecimal; // check this
    case KEY_NUMPAD_MULT: return Key::KeypadMultiply;
    case KEY_NUMPAD_PLUS: return Key::KeypadAdd;
    case KEY_NUMLOCK: return Key::NumLock;
    case KEY_NUMPAD_DIV: return Key::KeypadDivide;
    case KEY_NUMPAD_ENTER: return Key::KeypadEnter;
    case KEY_NUMPAD_MINUS: return Key::KeypadSubstract;
    case KEY_NUMPAD_0: return Key::Keypad_0;
    case KEY_NUMPAD_1: return Key::Keypad_1;
    case KEY_NUMPAD_2: return Key::Keypad_2;
    case KEY_NUMPAD_3: return Key::Keypad_3;
    case KEY_NUMPAD_4: return Key::Keypad_4;
    case KEY_NUMPAD_5: return Key::Keypad_5;
    case KEY_NUMPAD_6: return Key::Keypad_6;
    case KEY_NUMPAD_7: return Key::Keypad_7;
    case KEY_NUMPAD_8: return Key::Keypad_8;
    case KEY_NUMPAD_9: return Key::Keypad_9;
    case KEY_MENU: return Key::Menu;
    case KEY_HOME: return Key::Home;
    case KEY_END: return Key::End;
    case KEY_PAGEUP: return Key::PageUp;
    case KEY_PAGEDOWN: return Key::PageDown;
    case KEY_UP: return Key::Up;
    case KEY_DOWN: return Key::Down;
    case KEY_LEFT: return Key::Left;
    case KEY_RIGHT: return Key::Right;
    case KEY_F1: return Key::F1;
    case KEY_F2: return Key::F2;
    case KEY_F3: return Key::F3;
    case KEY_F4: return Key::F4;
    case KEY_F5: return Key::F5;
    case KEY_F6: return Key::F6;
    case KEY_F7: return Key::F7;
    case KEY_F8: return Key::F8;
    case KEY_F9: return Key::F9;
    case KEY_F10: return Key::F10;
    case KEY_F11: return Key::F11;
    case KEY_F12: return Key::F12;
    case KEY_F13: return Key::F13;
    case KEY_F14: return Key::F14;
    case KEY_F15: return Key::F15;
    default:
        return Key::Unknown;
    }
}

}//namespace MacOS::
