#include "platform/keyboard.hpp"
#include "platform/macos/input_manager.hpp"

namespace Keyboard{

bool IsKeyPressed(Key code){
    return MacOS::InputManager::s_KeyboardState[(size_t)code];
}

}//namespace Keyboard::