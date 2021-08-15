#include "platform/macos/window_impl.hpp"
#include "platform/macos/sx_window.h"
#include "platform/macos/sx_view.h"
#include "platform/macos/sx_window_delegate.h"
#include "platform/macos/linear_units.h"
#include "platform/macos/sx_application.h"

namespace MacOS{

Result WindowImpl::Open(int width, int height, const char *title){
    width = PixelsToLinearUnits(width, [NSScreen mainScreen]);
    height = PixelsToLinearUnits(height, [NSScreen mainScreen]);

    m_Handle = [[SXWindow alloc]initWithWidth:width Height:height WindowImpl:this];
    if(!m_Handle)
        return Result::Failure;

    m_View = [[SXView alloc]initWithWidht:width Height:height WindowImpl:this];
    if(!m_View)
        return Result::Failure;

    //should not return nil
    m_Delegate = [[SXWindowDelegate alloc]initWithWindowImpl:this];
    
    [(SXWindow*)m_Handle setDelegate: (SXWindowDelegate*)m_Delegate];
    [(SXWindow*)m_Handle setContentView: (SXView*)m_View];
    [(SXWindow*)m_Handle setInitialFirstResponder: (SXView*)m_View];
    [(SXWindow*)m_Handle setNextResponder: (SXView*)m_View];
    [(SXWindow*)m_Handle makeFirstResponder: (SXView*)m_View];
    [(SXWindow*)m_Handle center];//TODO replicate on other OSes
    [(SXWindow*)m_Handle makeKeyWindow];
    [(SXWindow*)m_Handle setOpaque:YES];
    [(SXWindow*)m_Handle makeKeyAndOrderFront: nil];

    SetTitle(title);

    return Result::Success;
}

void WindowImpl::Close(){
    [(SXWindow*)m_Handle release];
    [(SXView*)m_View release];
    m_Handle = nullptr;
}

bool WindowImpl::IsOpen()const{
    return m_Handle != nullptr;
}

void WindowImpl::SetEventsHandler(Function<void (const Event &)> handler){
    m_EventsHandler = handler;
}

void WindowImpl::DispatchEvents(){
    [SXApplication poolEvents];
}

void WindowImpl::SetTitle(const char *title){
    [(SXWindow*)m_Handle setTitle: [NSString stringWithUTF8String: title]];
}

Vector2u WindowImpl::Size()const{
    Vector2u size = {(u32)((SXWindow*)m_Handle).contentView.frame.size.width, (u32)((SXWindow*)m_Handle).contentView.frame.size.height};
    
    size.x = LinearUnitsToPixels(size.x, [(SXWindow*)m_Handle screen]);
    size.y = LinearUnitsToPixels(size.y, [(SXWindow*)m_Handle screen]);

    return size;
}

void WindowImpl::SetSize(u32 width, u32 height){
    width = PixelsToLinearUnits(width, [(SXWindow*)m_Handle screen]);
    height = PixelsToLinearUnits(height, [(SXWindow*)m_Handle screen]);

    NSRect frame = [(SXWindow*)m_Handle frame];
    frame.size.width = width;
    frame.size.height = height;

    [(SXWindow*)m_Handle setFrame: frame display: YES animate: YES];
}

const Screen &WindowImpl::CurrentScreen()const{
    //TODO for now macos supports only one screen
    if(m_CurrentScreen.Handle != [NSScreen mainScreen]){
        m_CurrentScreen.Handle = [NSScreen mainScreen];

        NSRect size = [(NSScreen*)m_CurrentScreen.Handle convertRectToBacking: ((NSScreen*)m_CurrentScreen.Handle).frame];

        NSDictionary *description = [((NSScreen*)m_CurrentScreen.Handle)deviceDescription];
        NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
        CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);

        m_CurrentScreen.Size = {(s32)size.size.width, (s32)size.size.height};
        m_CurrentScreen.DPI = {LinearUnitsToPixels(float(displayPixelSize.width / displayPhysicalSize.width) * 25.4f, ((NSScreen*)m_CurrentScreen.Handle)), 
        LinearUnitsToPixels(float(displayPixelSize.height / displayPhysicalSize.height) * 25.4f, ((NSScreen*)m_CurrentScreen.Handle))};
    }
    return m_CurrentScreen;
}

}//namespace MacOS::