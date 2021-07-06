#include <CoreGraphics/CoreGraphics.h>
#include <Cocoa/Cocoa.h>
#include "platform/mouse.hpp"
#include "platform/window.hpp"
#include "platform/window_system.hpp"
#include "platform/macos/sx_application.hpp"
#include "platform/macos/window_impl.hpp"
#include "platform/macos/sx_window.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button){
    MacOS::SXApplication::ProcessEvents();
    return MacOS::SXApplication::s_MouseState[button];
}

Point2i Mouse::GlobalPosition(){
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);

    auto screen_size = WindowSystem::MainScreen().Size();
    return {(i32)cursor.x, screen_size.height - (i32)cursor.y};
}

Point2i Mouse::RelativePosition(const PlatformWindow &window){
    auto global_pos = GlobalPosition();
    SXWindow *win_impl = static_cast<MacOS::SXWindowWrapper*>(window.Impl().Handle)->Handle;
    Point2i win_pos = {(i32)[win_impl frame].origin.x, (i32)[win_impl frame].origin.y};
    return {global_pos.x - win_pos.x, global_pos.y - win_pos.y};
}

void Mouse::SetGlobalPosition(const Point2i &position){
    CGWarpMouseCursorPosition(CGPointMake(position.x, position.y));
}

void Mouse::SetVisible(bool is_visible){
    if(is_visible)
        [NSCursor unhide];
    else
        [NSCursor hide];
}