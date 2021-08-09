#include "platform/macos/window_impl.hpp"
#include "platform/macos/sx_window.h"
#include "platform/macos/sx_view.h"
#include "platform/macos/sx_window_delegate.h"
#include "platform/macos/linear_units.h"

namespace MacOS{

WindowImpl WindowImpl::s_MainWindow;

Result WindowImpl::Open(int width, int height){
    width = PixelsToLinearUnits(width, [NSScreen mainScreen]);
    height = PixelsToLinearUnits(height, [NSScreen mainScreen]);

    Handle = [[SXWindow alloc]initWithWidth:width Height:height WindowImpl:this];
    if(!Handle)
        return Result::Failure;

    View = [[SXView alloc]initWithWidht:width Height:height WindowImpl:this];
    if(!View)
        return Result::Failure;

    //should not return nil
    Delegate = [[SXWindowDelegate alloc]initWithWindowImpl:this];
    
    [(SXWindow*)Handle setDelegate: (SXWindowDelegate*)Delegate];
    [(SXWindow*)Handle setContentView: (SXView*)View];
    [(SXWindow*)Handle setInitialFirstResponder: (SXView*)View];
    [(SXWindow*)Handle setNextResponder: (SXView*)View];
    [(SXWindow*)Handle makeFirstResponder: (SXView*)View];
    [(SXWindow*)Handle center];//TODO replicate on other OSes
    [(SXWindow*)Handle makeKeyWindow];
    [(SXWindow*)Handle setOpaque:YES];
    [(SXWindow*)Handle makeKeyAndOrderFront: nil];

    return Result::Success;
}

void WindowImpl::Close(){
    [(SXWindow*)Handle release];
    [(SXView*)View release];
}

void WindowImpl::SetTitle(const char *title){
    [(SXWindow*)Handle setTitle: [NSString stringWithUTF8String: title]];
}

Vector2u WindowImpl::Size()const{
    Vector2u size = {(u32)((SXWindow*)Handle).contentView.frame.size.width, (u32)((SXWindow*)Handle).contentView.frame.size.height};
    
    size.x = LinearUnitsToPixels(size.x, [(SXWindow*)Handle screen]);
    size.y = LinearUnitsToPixels(size.y, [(SXWindow*)Handle screen]);

    return size;
}

void WindowImpl::SetSize(u32 width, u32 height){
    width = PixelsToLinearUnits(width, [(SXWindow*)Handle screen]);
    height = PixelsToLinearUnits(height, [(SXWindow*)Handle screen]);

    NSRect frame = [(SXWindow*)Handle frame];
    frame.size.width = width;
    frame.size.height = height;

    [(SXWindow*)Handle setFrame: frame display: YES animate: YES];
}

const PlatformScreen &WindowImpl::Screen(){
    //TODO for now macos supports only one screen
    if(CurrentScreen.Handle != [NSScreen mainScreen]){
        CurrentScreen.Handle = [NSScreen mainScreen];

        NSRect size = [(NSScreen*)CurrentScreen.Handle convertRectToBacking: ((NSScreen*)CurrentScreen.Handle).frame];

        NSDictionary *description = [((NSScreen*)CurrentScreen.Handle)deviceDescription];
        NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
        CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);

        CurrentScreen.Size = {(s32)size.size.width, (s32)size.size.height};
        CurrentScreen.DPI = {LinearUnitsToPixels(float(displayPixelSize.width / displayPhysicalSize.width) * 25.4f, ((NSScreen*)CurrentScreen.Handle)), 
        LinearUnitsToPixels(float(displayPixelSize.height / displayPhysicalSize.height) * 25.4f, ((NSScreen*)CurrentScreen.Handle))};
    }
    return CurrentScreen;
}

}//namespace MacOS::