#include "platform/mouse.hpp"
#include "platform/windows/hinstance.hpp"
#include "platform/windows/virtual_keys.hpp"
namespace StraitX {


bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Vector2i Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);
    return Vector2i(position.x,position.y);
}

}; // namespace StraitX::