#include "platform/mouse.hpp"
#include "platform/macos/sx_application.hpp"

namespace StraitX{

bool Mouse::IsButtonPressed(Mouse::Button button){
    MacOS::SXApplication::ProcessEvents();
    return MacOS::SXApplication::s_MouseState[button];
}

Point Mouse::GlobalPosition(){
    return {};
}

Point Mouse::RelativePosition(const Window &window){
    return {};
}

void Mouse::SetGlobalPosition(const Point &position){

}

void Mouse::SetVisible(bool is_visible){

}

}//namespace StraitX::