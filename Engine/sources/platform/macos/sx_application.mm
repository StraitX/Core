#import <Cocoa/Cocoa.h>
#include "platform/macos/sx_application.hpp"
#include "platform/macos/keys.hpp"

namespace StraitX{
namespace MacOS{

bool SXApplication::s_KeyboardState[Keyboard::KeyCount];

void SXApplication::ProcessEvents(){
    [NSApplication sharedApplication];
    NSEvent *e = nil;

    while((e = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:nil
                                               inMode:NSDefaultRunLoopMode
                                        dequeue:YES]) != nil){
        [NSApp sendEvent:e];

        if([e type] == NSEventTypeKeyDown)
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = true;
        else if([e type] == NSEventTypeKeyUp)
            s_KeyboardState[ToStraitXKeyCode([e keyCode])] = false;
        else;
    }

}

}//namespace MacOS::
}//namespace StraitX::