#include "platform/keyboard.hpp"
#include "platform/macos/sx_application.hpp"

namespace StraitX{
namespace Keyboard{

bool IsKeyPressed(KeyCode code){
    MacOS::SXApplication::ProcessEvents();
    return MacOS::SXApplication::s_KeyboardState[code];
}

} // namespace Keyboard::
} // namespace StraitX::