#include <windows.h>
#include "core/os/keyboard.hpp"
#include "platform/windows/virtual_keys.hpp"

namespace Keyboard{

bool IsKeyPressed(Key code){
	return (GetAsyncKeyState(Windows::KeyCodeToVirtualKey(code)) & 0x8000) != 0;	
}

}// namespace Keyboard::