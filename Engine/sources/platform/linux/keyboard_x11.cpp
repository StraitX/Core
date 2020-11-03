
#include "platform/keyboard.hpp"
#include "platform/linux/display.hpp"
#include "platform/linux/keys_x11.hpp"
#include <X11/Xlib.h>
#include "unistd.h"
namespace StraitX{
namespace Keyboard{

bool IsKeyPressed(KeyCode code){
    ::Display *display = Linux::Display::Instance().Handle();

    KeySym sym = Linux::StraitXKeyToXKeySym(code);
    ::KeyCode keyCode = XKeysymToKeycode(display,sym);  

    if(keyCode != 0){
        char keymap[32];
        XQueryKeymap(display,keymap);
        return (keymap[keyCode/8] & (1 << (keyCode%8))) != 0;
    }else{
        write(1,"Keyboard:: shit has happened",29);
        return false;
    }
}

}; // namespace Keyboard::
}; // namespace StraitX::