#import <Cocoa/Cocoa.h>
#include "core/os/mouse.hpp"
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

    auto screen_size = MacOS::WindowImpl::s_MainWindow.Screen().Size;
    NSScreen* screen = ((SXWindow*)MacOS::WindowImpl::s_MainWindow.Handle).screen;
    return {LinearUnitsToPixels((s32)cursor.x, screen), screen_size.y - LinearUnitsToPixels((s32)cursor.y, screen)};
}

Vector2s Mouse::RelativePosition(){
    auto global_pos = GlobalPosition();
    SXWindow* win_impl = (SXWindow*)MacOS::WindowImpl::s_MainWindow.Handle;
    NSScreen* screen = win_impl.screen;
    Vector2s win_pos = {(s32)[win_impl frame].origin.x, (s32)[win_impl frame].origin.y};
    return {global_pos.x - LinearUnitsToPixels(win_pos.x, screen), global_pos.y - LinearUnitsToPixels(win_pos.y, screen)};
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