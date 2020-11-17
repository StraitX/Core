
#include "platform/keyboard.hpp"
#include "platform/linux/display_x11.hpp"
#include "platform/linux/keys_x11.hpp"
#include <X11/Xlib.h>

namespace StraitX{
namespace Keyboard{

static Linux::DisplayX11 s_Display;


void Initialize(const Linux::DisplayX11 &display){
    s_Display = display;
}

bool IsKeyPressed(KeyCode code){
    ::Display *display = s_Display.Handle();

    KeySym sym = Linux::KeyCodeToXKeySym(code);
    ::KeyCode keyCode = XKeysymToKeycode(display,sym);  

    if(keyCode != 0){
        char keymap[32];
        XQueryKeymap(display,keymap);
        return (keymap[keyCode/8] & (1 << (keyCode%8))) != 0;
    }else{
        return false;
    }
}

}; // namespace Keyboard::
}; // namespace StraitX::