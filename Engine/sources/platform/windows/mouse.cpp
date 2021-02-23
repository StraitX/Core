#include "platform/mouse.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "platform/windows/window_impl.hpp"
#include "platform/window.hpp"
namespace StraitX {


bool Mouse::IsButtonPressed(Mouse::Button button) {
    return (GetAsyncKeyState(Windows::MouseButtonToVirtualKey(button)) & 0x8000) != 0;
}

Point Mouse::GlobalPosition() {
    POINT position = { 0 };
    GetCursorPos(&position);
    return {position.x,position.y};
}

Point Mouse::RelativePosition(const Window &window){
    // TODO: Make sure it works
    POINT position = { 0 };
    GetCursorPos(&position);
    ScreenToClient(window.Impl().Handle(),&position);
    return {position.x, position.y};
}

void Mouse::SetGlobalPosition(const Point &position){
    SetCursorPos(position.x, position.y);
}
void Mouse::SetVisible(bool is_visible, const Window &window){
    // TODO: implement
}

}; // namespace StraitX::