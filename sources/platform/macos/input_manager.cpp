#include "platform/macos/input_manager.hpp"
#include "core/os/memory.hpp"
namespace MacOS{

bool InputManager::s_KeyboardState[(size_t)Key::KeysCount] = {};
bool InputManager::s_MouseState[(size_t)Mouse::Button::ButtonCount] = {};

void InputManager::ResetKeyboardState(){
    Memory::Set(s_KeyboardState, 0, sizeof(s_KeyboardState));
}

void InputManager::ResetMouseState(){
    Memory::Set(s_MouseState, 0, sizeof(s_MouseState));
}

}//namespace MacOS::