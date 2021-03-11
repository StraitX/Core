#import <Cocoa/Cocoa.h>
#include "platform/window_system.hpp"

namespace StraitX{

Result WindowSystem::Initialize(){
    if(NSApplicationLoad() != YES)
        return Result::Failure;
    [NSApplication sharedApplication];
    return ResultError(NSApp == nil);
}

Result WindowSystem::Finalize(){
    return Result::Success;
}

Screen WindowSystem::MainScreen(){
    return {{}};
}

}//namespace StraitX::