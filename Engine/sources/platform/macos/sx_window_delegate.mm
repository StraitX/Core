#include "platform/macos/sx_window_delegate.h"


@implementation SXWindowDelegate

-(instancetype)initWithWindowImpl:(MacOS::WindowImpl*)impl{
    WindowImpl = impl;

    return self;
}

-(BOOL)windowShouldClose:(id)sender{
    Event e;
    e.Type = EventType::WindowClose;

    WindowImpl->EventHandler(e);

    return NO;
}

-(void)windowDidResize:(NSNotification*)notification{
    auto size = WindowImpl->Size();

    Event e;
    e.Type = EventType::WindowResized;
    e.WindowResized.x = size.width;
    e.WindowResized.y = size.height;

    WindowImpl->EventHandler(e);
}

@end