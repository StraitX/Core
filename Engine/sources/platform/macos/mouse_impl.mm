#include <CoreGraphics/CoreGraphics.h>
#include <Cocoa/Cocoa.h>
#include "platform/mouse.hpp"
#include "platform/window.hpp"
#include "platform/macos/sx_application.hpp"
#include "platform/macos/window_impl.hpp"
#include "platform/macos/sx_window.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button){
    MacOS::SXApplication::ProcessEvents();
    return MacOS::SXApplication::s_MouseState[button];
}

Point Mouse::GlobalPosition(){
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);
    return {(s32)cursor.x, (s32)cursor.y};
}

Point Mouse::RelativePosition(const Window &window){
    auto global_pos = GlobalPosition();
    SXWindow *win_impl = static_cast<MacOS::SXWindowWrapper*>(window.Impl().Handle)->Handle;
    Point win_pos = {(s32)[win_impl frame].origin.x, (s32)[NSScreen mainScreen].frame.size.height - (s32)[win_impl frame].origin.y - (s32)win_impl.contentView.frame.size.height};
    return {global_pos.x - win_pos.x, global_pos.y - win_pos.y};
}

void Mouse::SetGlobalPosition(const Point &position){
    CGWarpMouseCursorPosition(CGPointMake(position.x, position.y));
}

void Mouse::SetVisible(bool is_visible){
    if(is_visible)
        [NSCursor unhide];
    else
        [NSCursor hide];
}