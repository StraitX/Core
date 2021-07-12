#include "platform/keyboard.hpp"
#include "platform/macos/sx_application.hpp"

namespace Keyboard{

bool IsKeyPressed(Key code){
    MacOS::SXApplication::HandleEvents();
    return MacOS::SXApplication::s_KeyboardState[(size_t)code];
}

}//namespace Keyboard::