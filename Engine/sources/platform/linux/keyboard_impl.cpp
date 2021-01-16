#include "platform/keyboard.hpp"
#include "platform/linux/keys.hpp"
#include <X11/Xlib.h>

namespace StraitX{

namespace Linux{
extern ::Display *s_Display;
}//namespace Linux::

namespace Keyboard{

bool IsKeyPressed(KeyCode code){
    KeySym sym = Linux::KeyCodeToXKeySym(code);
    ::KeyCode keyCode = XKeysymToKeycode(Linux::s_Display,sym);  

    if(keyCode != 0){
        char keymap[32];
        XQueryKeymap(Linux::s_Display,keymap);
        return (keymap[keyCode/8] & (1 << (keyCode%8))) != 0;
    }else{
        return false;
    }
}

} // namespace Keyboard::
} // namespace StraitX::