#include "platform/mouse.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"
#include "platform/window.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Point2i Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);

    auto screen_size = PlatformWindow::Screen().Size;

    position.y = screen_size.height - position.y;
    return {position.x,position.y};
}

Point2i Mouse::RelativePosition(const PlatformWindow &window){
    POINT position = { 0 };
    GetCursorPos(&position);
    ScreenToClient(window.Impl().Handle(),&position);

    position.y = window.Size().height - position.y;
    return {position.x, position.y};
}

void Mouse::SetGlobalPosition(const Point2i &position){
    
    Point2i new_position;
    new_position.x = position.x;
    new_position.y = PlatformWindow::Screen().Size.height - position.y;

    SetCursorPos(new_position.x, new_position.y);
}
void Mouse::SetVisible(bool is_visible){
    ShowCursor(is_visible);
}