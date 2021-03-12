#import <Cocoa/Cocoa.h>
#include "platform/window_system.hpp"

namespace StraitX{

Result WindowSystem::Initialize(){

    NSApplication *app = [NSApplication sharedApplication];
    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
    [app activateIgnoringOtherApps: YES];

    return ResultError(NSApp == nil);
}

Result WindowSystem::Finalize(){
    return Result::Success;
}

Screen WindowSystem::MainScreen(){
    return {{}};
}

}//namespace StraitX::