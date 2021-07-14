#include "platform/macos/sx_window.h"
#include "platform/macos/input_manager.hpp"

@implementation SXWindow

-(instancetype)initWithWidth: (int)width Height: (int)height WindowImpl:(MacOS::WindowImpl *)impl{
    NSRect frame = NSMakeRect(0, 0, width, height);

    self = [super initWithContentRect:frame
            styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
            defer:NO];
    if(self == nil)return nil;

    WindowImpl = impl;

    [self setBackgroundColor:[NSColor blueColor]];
    [self makeKeyAndOrderFront:[NSApplication sharedApplication]];
    [self makeKeyWindow];
    [self makeMainWindow];

    return self;
}

-(void)dealloc{
    [self close];
    [super dealloc];
}

-(BOOL)canBecomeKeyWindow{
    return YES;
}

-(BOOL)canBecomeMainWindow{
    return YES;
}

-(void)becomeKeyWindow{
    Event e;
    e.Type = EventType::FocusIn;

    WindowImpl->EventHandler(e);
}

-(void)resignKeyWindow{
    Event e;
    e.Type = EventType::FocusOut;

    WindowImpl->EventHandler(e);

    MacOS::InputManager::ResetKeyboardState();
    MacOS::InputManager::ResetMouseState();
}

@end