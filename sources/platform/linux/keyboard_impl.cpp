#include "core/os/keyboard.hpp"
#include "platform/linux/keys.hpp"
#include "platform/linux/display_server.hpp"

namespace Keyboard{

using namespace Linux;

bool IsKeyPressed(Key code){
    X11::KeySym sym = KeyCodeToXKeySym(code);
    X11::KeyCode keyCode = XKeysymToKeycode(DisplayServer::Handle, sym);  

    if(keyCode != 0){
        char keymap[32];
        XQueryKeymap(DisplayServer::Handle,keymap);
        return (keymap[keyCode/8] & (1 << (keyCode%8))) != 0;
    }else{
        return false;
    }
}

}//namespace Keyboard::