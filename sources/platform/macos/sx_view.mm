#include "platform/macos/sx_view.h"
#include "platform/macos/linear_units.h"
#include "platform/macos/keys.hpp"

@implementation SXView

-(instancetype)initWithWidht: (int)width Height: (int)height WindowImpl:(MacOS::WindowImpl *)impl{
    NSRect size = NSMakeRect(0, 0, width, height);

    self = [super initWithFrame: size];

    if(self == nil)return nil;

    WindowImpl = impl;

    return self;
}

-(void)mouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];

    [self onPressedButton: Mouse::Left X: pos.x Y: pos.y];
}

-(void)mouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];

    [self onReleasedButton: Mouse::Left X: pos.x Y: pos.y];
}

-(void)rightMouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    [self onPressedButton: Mouse::Right X: pos.x Y: pos.y];
}

-(void)rightMouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    [self onReleasedButton: Mouse::Right X: pos.x Y: pos.y];
}

-(void)otherMouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    [self onPressedButton: Mouse::Middle X: pos.x Y: pos.y];
}

-(void)otherMouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    [self onReleasedButton: Mouse::Middle X: pos.x Y: pos.y];
}

-(BOOL)canBecomeKeyView{
    return YES;
}

-(BOOL)acceptsFirstResponder{
    return YES;
}

-(BOOL)wantsUpdateLayer{
    return YES;
}

-(void)keyDown:(NSEvent *)event{
    Event e;
    e.Type = EventType::KeyPress;
    e.KeyPress.KeyCode = MacOS::ToStraitXKeyCode([event keyCode]);

    WindowImpl->EventsHandler().TryCall(e);
}

-(void)keyUp:(NSEvent *)event{
    Event e;
    e.Type = EventType::KeyPress;
    e.KeyPress.KeyCode = MacOS::ToStraitXKeyCode([event keyCode]);

    WindowImpl->EventsHandler().TryCall(e);
}
//Handles Ctrl shift alt and others
-(void)flagsChanged:(NSEvent*)event{
    (void)event;
    //TODO: deal with this value
    if([event modifierFlags] == 256){
        Event e;
        e.Type = EventType::KeyRelease;
        e.KeyRelease.KeyCode = MacOS::ToStraitXKeyCode([event keyCode]);
        WindowImpl->EventsHandler().TryCall(e);
    }else{
        Event e;
        e.Type = EventType::KeyPress;
        e.KeyPress.KeyCode = MacOS::ToStraitXKeyCode([event keyCode]);
        WindowImpl->EventsHandler().TryCall(e);
    }
}

-(void)scrollWheel:(NSEvent *)event{
    double deltaY = [event scrollingDeltaY];

    if ([event hasPreciseScrollingDeltas]){
        deltaY *= 0.1;
    }

    if((int)deltaY){
        Event e;
        e.Type = EventType::MouseWheel;
        e.MouseWheel.Delta = (int)deltaY;

        WindowImpl->EventsHandler().TryCall(e);
    }
}

-(void)mouseDragged:(NSEvent *)event{
    (void)event;
}

-(void)rightMouseDragged:(NSEvent *)event{
    (void)event;
}

-(void)otherMouseDragged:(NSEvent *)event{
    (void)event;
}

-(void)onPressedButton:(Mouse::Button)button X: (int)x Y: (int)y{
    x = LinearUnitsToPixels(x, ((NSWindow*)WindowImpl->Handle()).screen);
    y = LinearUnitsToPixels(y, ((NSWindow*)WindowImpl->Handle()).screen);

    Event e;
    e.Type = EventType::MouseButtonPress;
    e.MouseButtonPress.x = x;
    e.MouseButtonPress.y = y;

    WindowImpl->EventsHandler().TryCall(e);
}

-(void)onReleasedButton:(Mouse::Button)button X: (int)x Y: (int)y{
    x = LinearUnitsToPixels(x, ((NSWindow*)WindowImpl->Handle()).screen);
    y = LinearUnitsToPixels(y, ((NSWindow*)WindowImpl->Handle()).screen);

    Event e;
    e.Type = EventType::MouseButtonRelease;
    e.MouseButtonPress.x = x;
    e.MouseButtonPress.y = y;

    WindowImpl->EventsHandler().TryCall(e);
}

@end