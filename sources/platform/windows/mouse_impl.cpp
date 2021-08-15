#include "core/os/mouse.hpp"
#include "core/os/window.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Vector2s Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);

    return {position.x,position.y};
}

Vector2s Mouse::RelativePosition(const Window &window){
    POINT position = { 0 };
    GetCursorPos(&position);
    ScreenToClient(window.Impl().Handle(),&position);

    return {position.x, position.y};
}

void Mouse::SetGlobalPosition(const Vector2s &position){
    SetCursorPos(position.x, position.y);
}
void Mouse::SetVisible(bool is_visible){
    ShowCursor(is_visible);
}