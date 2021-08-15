#import <Cocoa/Cocoa.h>
#include "core/os/mouse.hpp"
#include "core/os/window.hpp"
#include "platform/macos/sx_window.h"
#include "platform/macos/linear_units.h"
#include "platform/macos/input_manager.hpp"
#include "platform/macos/window_impl.hpp"

bool Mouse::IsButtonPressed(Mouse::Button button){
    return MacOS::InputManager::s_MouseState[(size_t)button];
}

Vector2s Mouse::GlobalPosition(){
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);

    NSScreen* screen = [NSScreen mainScreen];

    return {LinearUnitsToPixels((s32)cursor.x, screen), LinearUnitsToPixels((s32)cursor.y, screen)};
}

Vector2s Mouse::RelativePosition(const Window &window){
    auto global_pos = GlobalPosition();
    SXWindow* win_impl = (SXWindow*)window.Impl().Handle();
    NSScreen* screen = win_impl.screen;
    Vector2s win_pos = {(s32)[win_impl frame].origin.x, (s32)[win_impl frame].origin.y};
    win_pos.x = LinearUnitsToPixels(win_pos.x, screen);
    win_pos.y = window.CurrentScreen().Size.y - (LinearUnitsToPixels(win_pos.y, screen) + window.Size().y) + 2;

    return {global_pos.x - win_pos.x, global_pos.y - win_pos.y};
}

void Mouse::SetGlobalPosition(const Vector2s &position){
    CGWarpMouseCursorPosition(CGPointMake(position.x, position.y));
}


void Mouse::SetVisible(bool is_visible){
    if(is_visible)
        [NSCursor unhide];
    else
        [NSCursor hide];
}