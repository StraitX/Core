#include "platform/macos/keys.hpp"

namespace StraitX{
namespace MacOS{

Keyboard::KeyCode ToStraitXKeyCode(int key_code){
    switch (key_code){
    case KEY_A: return Keyboard::A;
    case KEY_S: return Keyboard::S;
    case KEY_D: return Keyboard::D;
    case KEY_F: return Keyboard::F;
    case KEY_H: return Keyboard::H;
    case KEY_G: return Keyboard::G;
    case KEY_Z: return Keyboard::Z;
    case KEY_X: return Keyboard::X;
    case KEY_C: return Keyboard::C;
    case KEY_V: return Keyboard::V;
    case KEY_B: return Keyboard::B;
    case KEY_Q: return Keyboard::Q;
    case KEY_W: return Keyboard::W;
    case KEY_E: return Keyboard::E;
    case KEY_R: return Keyboard::R;
    case KEY_Y: return Keyboard::Y;
    case KEY_T: return Keyboard::T;
    case KEY_1: return Keyboard::Key_1;
    case KEY_2: return Keyboard::Key_2;
    case KEY_3: return Keyboard::Key_3;
    case KEY_4: return Keyboard::Key_4;
    case KEY_6: return Keyboard::Key_6;
    case KEY_5: return Keyboard::Key_5;
    case KEY_EQUALS: return Keyboard::Equal;
    case KEY_9: return Keyboard::Key_9;
    case KEY_7: return Keyboard::Key_7;
    case KEY_MINUS: return Keyboard::Minus;
    case KEY_8: return Keyboard::Key_8;
    case KEY_0: return Keyboard::Key_0;
    case KEY_RIGHTBRACKET: return Keyboard::RightBracket;
    case KEY_O: return Keyboard::O;
    case KEY_U: return Keyboard::U;
    case KEY_LEFTBRACKET: return Keyboard::LeftBracket;
    case KEY_I: return Keyboard::I;
    case KEY_P: return Keyboard::P;
    case KEY_RETURN: return Keyboard::Enter;
    case KEY_L: return Keyboard::L;
    case KEY_J: return Keyboard::J;
    case KEY_APOSTROPHE: return Keyboard::Quote;
    case KEY_K: return Keyboard::K;
    case KEY_SEMICOLON: return Keyboard::Semicolon;
    case KEY_FRONTSLASH: return Keyboard::Slash;
    case KEY_COMMA: return Keyboard::Comma;
    case KEY_BACKSLASH: return Keyboard::Backslash;
    case KEY_N: return Keyboard::N;
    case KEY_M: return Keyboard::M;
    case KEY_PERIOD: return Keyboard::Period;
    case KEY_TAB: return Keyboard::Tab;
    case KEY_SPACE: return Keyboard::Space;
    case KEY_BACKAPOSTROPHE: return Keyboard::GraveAccent;
    case KEY_DELETE_BACKSPACE: return Keyboard::Backspace;
    case KEY_DELETE: return Keyboard::Delete;
    case KEY_ESCAPE: return Keyboard::Escape;
    case KEY_RIGHTCOMMAND: return Keyboard::RightSuper;
    case KEY_LEFTCOMMAND: return Keyboard::LeftSuper;
    case KEY_LEFTSHIFT: return Keyboard::LeftShift;
    case KEY_CAPSLOCK: return Keyboard::CapsLock;
    case KEY_LEFTALT: return Keyboard::LeftAlt;
    case KEY_LEFTCONTROL: return Keyboard::LeftControl;
    case KEY_RIGHTSHIFHT: return Keyboard::RightShift;
    case KEY_RIGHTALT: return Keyboard::RightAlt;
    case KEY_RIGHTCTRL: return Keyboard::RightControl;
    case KEY_HOME: return Keyboard::Home;
    case KEY_END: return Keyboard::End;
    case KEY_PAGEUP: return Keyboard::PageUp;
    case KEY_PAGEDOWN: return Keyboard::PageDown;
    case KEY_UP: return Keyboard::Up;
    case KEY_DOWN: return Keyboard::Down;
    case KEY_LEFT: return Keyboard::Left;
    case KEY_RIGHT: return Keyboard::Right;
    case KEY_F1: return Keyboard::F1;
    case KEY_F2: return Keyboard::F2;
    case KEY_F3: return Keyboard::F3;
    case KEY_F4: return Keyboard::F4;
    case KEY_F5: return Keyboard::F5;
    case KEY_F6: return Keyboard::F6;
    case KEY_F7: return Keyboard::F7;
    case KEY_F8: return Keyboard::F8;
    case KEY_F9: return Keyboard::F9;
    case KEY_F10: return Keyboard::F10;
    case KEY_F11: return Keyboard::F11;
    case KEY_F12: return Keyboard::F12;
    case KEY_F13: return Keyboard::F13;
    case KEY_F14: return Keyboard::F14;
    case KEY_F15: return Keyboard::F15;
    default:
        return Keyboard::Unknown;
    }
}

}//namespace MacOS::
}//namespace StraitX::