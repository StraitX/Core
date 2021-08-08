#include "core/os/mouse.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"
#include "platform/windows/window_impl.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Vector2s Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);

    auto screen_size = Windows::WindowImpl::s_MainWindow.Screen().Size;

    position.y = screen_size.y - position.y;
    return {position.x,position.y};
}

Vector2s Mouse::RelativePosition(){
    POINT position = { 0 };
    GetCursorPos(&position);
    ScreenToClient(Windows::WindowImpl::s_MainWindow.Handle(),&position);

    position.y = Windows::WindowImpl::s_MainWindow.Size().y - position.y;
    return {position.x, position.y};
}

void Mouse::SetGlobalPosition(const Vector2s &position){
    
    Vector2s new_position;
    new_position.x = position.x;
    new_position.y = Windows::WindowImpl::s_MainWindow.Screen().Size.y - position.y;

    SetCursorPos(new_position.x, new_position.y);
}
void Mouse::SetVisible(bool is_visible){
    ShowCursor(is_visible);
}