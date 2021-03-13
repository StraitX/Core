#include "platform/macos/sx_window.hpp"
#include "platform/macos/keys.hpp"

@implementation SXView

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height{
    NSRect frame = NSMakeRect(0, 0, width, height);
    self = [super initWithFrame:frame];

    if(self==nil)return nil;

    m_Wrapper = owner;

    return self;
}

-(void)mouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonPress(StraitX::Mouse::Left, pos.x, event.window.contentView.frame.size.height - pos.y);
}

-(void)mouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonRelease(StraitX::Mouse::Left,pos.x, event.window.contentView.frame.size.height - pos.y);
}

-(void)rightMouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonPress(StraitX::Mouse::Right,pos.x, event.window.contentView.frame.size.height - pos.y);
}

-(void)rightMouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonRelease(StraitX::Mouse::Right,pos.x, event.window.contentView.frame.size.height - pos.y);
}

-(void)otherMouseDown:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonPress(StraitX::Mouse::Middle,pos.x, event.window.contentView.frame.size.height - pos.y);
}

-(void)otherMouseUp:(NSEvent *)event{
    (void)event;
    NSPoint pos = [event locationInWindow];
    m_Wrapper->OnMouseButtonRelease(StraitX::Mouse::Middle,pos.x, event.window.contentView.frame.size.height - pos.y);
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
    m_Wrapper->OnKeyPress(StraitX::MacOS::ToStraitXKeyCode([event keyCode]));
}

-(void)keyUp:(NSEvent *)event{
    m_Wrapper->OnKeyRelease(StraitX::MacOS::ToStraitXKeyCode([event keyCode]));
}
-(void)flagsChanged:(NSEvent*)event{
    if([event modifierFlags] == 256)//TODO: deal with this value
        m_Wrapper->OnKeyRelease(StraitX::MacOS::ToStraitXKeyCode([event keyCode]));
    else
        m_Wrapper->OnKeyPress(StraitX::MacOS::ToStraitXKeyCode([event keyCode]));
}

-(void)scrollWheel:(NSEvent *)event{
    double deltaY = [event scrollingDeltaY];

    if ([event hasPreciseScrollingDeltas]){
        deltaY *= 0.1;
    }
    if((int)deltaY)//TODO take care of direction on macos
        m_Wrapper->OnMouseWheel((int)deltaY);
}

@end

@implementation SXWindowDelegate

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner{
    self = [super init];

    if(self == nil)return nil;

    m_Wrapper = owner;

    return self;
}

-(BOOL)windowShouldClose:(id)sender{
    m_Wrapper->OnWindowClose();
    return NO;
}

-(void)windowDidResize:(NSNotification*)notification{
    auto size = m_Wrapper->Size();
    m_Wrapper->OnWindowResized(size.width, size.height);
}

@end

@implementation SXWindow

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height{
    NSRect frame = NSMakeRect(0, 0, width, height);
    self = [super initWithContentRect:frame
            styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
            defer:NO];
    if(self == nil)
        return nil;
    m_Wrapper = owner;
    [self setBackgroundColor:[NSColor blueColor]];
    [self makeKeyAndOrderFront:[NSApplication sharedApplication]];
    [self makeKeyWindow];
    [self makeMainWindow];

    return self;
}

-(void)close{
    [self close];
}

-(void)processEvents{
    [NSApplication sharedApplication];
    NSEvent *e = nil;

    while((e = [NSApp nextEventMatchingMask:NSEventMaskAny
                                            untilDate:nil
                                               inMode:NSDefaultRunLoopMode
                                        dequeue:YES]) != nil){
        [NSApp sendEvent:e];
    }
}

-(BOOL)canBecomeKeyWindow{
    // Required for NSWindowStyleMaskBorderless windows
    return YES;
}

-(BOOL)canBecomeMainWindow{
    return YES;
}

@end

namespace StraitX{
namespace MacOS{

Result SXWindowWrapper::Open(const ScreenImpl &screen, int width, int height){
    Handle = [[SXWindow alloc] initWithSXWindow: this Width: width Height: height];

    if(Handle == nil)goto window_fail;

    Delegate = [[SXWindowDelegate alloc]initWithSXWindow: this]; 

    if(Delegate == nil)goto deleagete_fail;

    View = [[SXView alloc]initWithSXWindow: this Width: width Height: height];

    if(View == nil)goto view_fail;

    [Handle setDelegate: Delegate];
    [Handle setContentView: View];
    [Handle setInitialFirstResponder: View];
    [Handle setNextResponder: View];
    [Handle makeFirstResponder: View];
    [Handle center];//TODO replicate on other OSes
    [Handle makeKeyWindow];
    [Handle setOpaque:YES];
    [Handle makeKeyAndOrderFront: nil];

    return Result::Success;
view_fail:
    //somehow delete Delegate
deleagete_fail:
    [Handle close];
window_fail:
    return Result::Failure;
}

Result SXWindowWrapper::Close(){
    return Result::Success;
}

bool SXWindowWrapper::IsOpen()const{
    return true;
}

void SXWindowWrapper::SetTitle(const char *title){
    [Handle setTitle: [NSString stringWithUTF8String: title]];
}

bool SXWindowWrapper::PollEvent(Event &event){
    [Handle processEvents];

    if(!EventsQueue.empty()){
        event = EventsQueue.front();
        EventsQueue.pop();
        return true;
    }
    return false;
}

Size2u SXWindowWrapper::Size()const{
    return {(u32)Handle.contentView.frame.size.width, (u32)Handle.contentView.frame.size.height};
}

void SXWindowWrapper::OnWindowClose(){
    Event e;
    e.Type = EventType::WindowClose;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnWindowResized(u32 width, u32 height){
    Event e;
    e.Type = EventType::WindowResized;
    e.WindowResized.x = width;
    e.WindowResized.y = height;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnWindowDraw(){
    Event e;
    e.Type = EventType::WindowDraw;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnMouseWheel(u32 delta){
    Event e;
    e.Type = EventType::MouseWheel;
    e.MouseWheel.Delta = delta;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnMouseButtonPress(Mouse::Button button, s32 x, s32 y){
    Event e;
    e.Type = EventType::MouseButtonPress;
    e.MouseButtonPress.Button = button;
    e.MouseButtonPress.x = x;
    e.MouseButtonPress.y = y;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnMouseButtonRelease(Mouse::Button button, s32 x, s32 y){
    Event e;
    e.Type = EventType::MouseButtonRelease;
    e.MouseButtonPress.Button = button;
    e.MouseButtonPress.x = x;
    e.MouseButtonPress.y = y;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnKeyPress(Keyboard::KeyCode key){
    Event e;
    e.Type = EventType::KeyPress;
    e.KeyPress.KeyCode = key;
    EventsQueue.push(e);
}

void SXWindowWrapper::OnKeyRelease(Keyboard::KeyCode key){
    Event e;
    e.Type = EventType::KeyRelease;
    e.KeyPress.KeyCode = key;
    EventsQueue.push(e);
}

}//namespace MacOS::
}//namespace StraitX::
