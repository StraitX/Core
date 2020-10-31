#include <windows.h>
#include "platform/keyboard.hpp"
#include "platform/windows/virtual_keys.hpp"

namespace StraitX{
namespace Keyboard{

bool IsKeyPressed(KeyCode code){
	return (GetAsyncKeyState(Windows::KeyCodeToVirtualKey(code)) & 0x8000) != 0;	
}

}; // namespace Keyboard::
}; // namespace StraitX::