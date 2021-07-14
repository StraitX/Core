#include "platform/macos/sx_window.h"


@implementation SXWindow

-(instancetype)initWithWidth: (int)width Height: (int)height{
    NSRect frame = NSMakeRect(0, 0, width, height);

    self = [super initWithContentRect:frame
            styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
            defer:NO];
    if(self == nil)return nil;

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

@end