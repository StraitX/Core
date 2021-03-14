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
        if([e type] != NSEventTypePressure)
            [NSApp sendEvent:e];

        if([e type] == NSEventTypeKeyDown)
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = true;
        else if([e type] == NSEventTypeKeyUp)
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = false;
        else if([e type] == NSEventTypeFlagsChanged)
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = ([e modifierFlags] == 256) ? false : true;
        else if([e type] == NSEventTypeLeftMouseDown)
            s_MouseState[Mouse::Left] = true;
        else if([e type] == NSEventTypeRightMouseDown)
            s_MouseState[Mouse::Right] = true;
        else if([e type] == NSEventTypeOtherMouseDown)
            s_MouseState[Mouse::Middle] = true;
        else if([e type] == NSEventTypeLeftMouseUp)
            s_MouseState[Mouse::Left] = false;
        else if([e type] == NSEventTypeRightMouseUp)
            s_MouseState[Mouse::Right] = false;
        else if([e type] == NSEventTypeOtherMouseUp)
            s_MouseState[Mouse::Middle] = false;
    }

}

}//namespace MacOS::
}//namespace StraitX::