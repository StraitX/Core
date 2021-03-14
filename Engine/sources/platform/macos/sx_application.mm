#import <Cocoa/Cocoa.h>
#include "platform/macos/sx_application.hpp"
#include "platform/macos/keys.hpp"

namespace StraitX{
namespace MacOS{

bool SXApplication::s_KeyboardState[Keyboard::KeyCount];
bool SXApplication::s_MouseState[Mouse::ButtonCount];

void SXApplication::ProcessEvents(){
    [NSApplication sharedApplication];
    NSEvent *e = nil;

    while((e = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:nil
                                               inMode:NSDefaultRunLoopMode
                                        dequeue:YES]) != nil){
        switch ([e type]){
        case NSEventTypeKeyDown:
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = true;
            break;
        case NSEventTypeKeyUp:
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = false;
            break;
        case NSEventTypeFlagsChanged:
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = ([e modifierFlags] == 256) ? false : true;
            break;
        case NSEventTypeLeftMouseDown:
            s_MouseState[Mouse::Left] = true;
            break;
        case NSEventTypeRightMouseDown:
            s_MouseState[Mouse::Right] = true;
            break;
        case NSEventTypeOtherMouseDown:
            s_MouseState[Mouse::Middle] = true;
            break;
        case NSEventTypeLeftMouseUp:
            s_MouseState[Mouse::Left] = false;
            break;
        case NSEventTypeRightMouseUp:
            s_MouseState[Mouse::Right] = false;
            break;
        case NSEventTypeOtherMouseUp:
            s_MouseState[Mouse::Middle] = false;
            break;
        case NSEventTypePressure:
            continue;
        }
        [NSApp sendEvent:e];
    }

}

}//namespace MacOS::
}//namespace StraitX::