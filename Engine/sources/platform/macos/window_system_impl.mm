#import <Cocoa/Cocoa.h>
#include "platform/window_system.hpp"
#include "platform/macos/window_impl.hpp"
#include "platform/macos/linear_units.h"

namespace MacOS{
    WindowImpl s_Window;
}//namespace MacOS::

Result WindowSystem::Initialize(int width, int height){

    NSApplication *app = [NSApplication sharedApplication];
    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
    [app activateIgnoringOtherApps: YES];

    return MacOS::s_Window.Open(MainScreen().Impl(), width, height);
}

void WindowSystem::Finalize(){
    MacOS::s_Window.Close();
}

PlatformScreen WindowSystem::MainScreen(){
    NSScreen *screen = [NSScreen mainScreen];

    NSRect size = [screen convertRectToBacking: screen.frame];

    NSDictionary *description = [screen deviceDescription];
    NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
    CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);

    return MacOS::ScreenImpl(
        screen,
        {(i32)size.size.width, (i32)size.size.height}, 
        {LinearUnitsToPixels(float(displayPixelSize.width / displayPhysicalSize.width) * 25.4f, screen), 
        LinearUnitsToPixels(float(displayPixelSize.height / displayPhysicalSize.height) * 25.4f, screen)}
    );
}

PlatformWindow WindowSystem::Window(){
    return MacOS::s_Window;
}
