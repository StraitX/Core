#include "platform/macos/sx_application.h"
#include "platform/macos/keys.hpp"
#include "platform/macos/input_manager.hpp"

using namespace MacOS;

@implementation SXApplication

+(void)poolEvents{
    @autoreleasepool{
        NSEvent *e = nil;

        while((e = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:nil
                                                inMode:NSDefaultRunLoopMode
                                            dequeue:YES]) != nil){
            switch ([e type]){
            case NSEventTypeKeyDown:
                InputManager::s_KeyboardState[(size_t)ToStraitXKeyCode([e keyCode])] = true;
                break;
            case NSEventTypeKeyUp:
                InputManager::s_KeyboardState[(size_t)ToStraitXKeyCode([e keyCode])] = false;
                break;
            case NSEventTypeFlagsChanged:
                InputManager::s_KeyboardState[(size_t)ToStraitXKeyCode([e keyCode])] = ([e modifierFlags] == 256) ? false : true;
                break;
            case NSEventTypeLeftMouseDown:
                InputManager::s_MouseState[Mouse::Left] = true;
                break;
            case NSEventTypeRightMouseDown:
                InputManager::s_MouseState[Mouse::Right] = true;
                break;
            case NSEventTypeOtherMouseDown:
                InputManager::s_MouseState[Mouse::Middle] = true;
                break;
            case NSEventTypeLeftMouseUp:
                InputManager::s_MouseState[Mouse::Left] = false;
                break;
            case NSEventTypeRightMouseUp:
                InputManager::s_MouseState[Mouse::Right] = false;
                break;
            case NSEventTypeOtherMouseUp:
                InputManager::s_MouseState[Mouse::Middle] = false;
                break;
            case NSEventTypePressure:
                continue;
            default:
                (void)0;
            }
            [NSApp sendEvent:e];
        }
    }
}

@end