#include "platform/mouse.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"
#include "platform/windows/window_impl.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Point2i Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);

    auto screen_size = Windows::WindowImpl::s_MainWindow.Screen().Size;

    position.y = screen_size.height - position.y;
    return {position.x,position.y};
}

Point2i Mouse::RelativePosition(){
    POINT position = { 0 };
    GetCursorPos(&position);
    ScreenToClient(Windows::WindowImpl::s_MainWindow.Handle(),&position);

    position.y = Windows::WindowImpl::s_MainWindow.Size().height - position.y;
    return {position.x, position.y};
}

void Mouse::SetGlobalPosition(const Point2i &position){
    
    Point2i new_position;
    new_position.x = position.x;
    new_position.y = Windows::WindowImpl::s_MainWindow.Screen().Size.height - position.y;

    SetCursorPos(new_position.x, new_position.y);
}
void Mouse::SetVisible(bool is_visible){
    ShowCursor(is_visible);
}