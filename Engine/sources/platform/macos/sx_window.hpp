#ifndef STRAITX_MACOS_SX_WINDOW_HPP
#define STRAITX_MACOS_SX_WINDOW_HPP

#include "platform/macos/common.hpp"
#import <Cocoa/Cocoa.h>
#include <queue>
#include "platform/result.hpp"
#include "platform/events.hpp"
#include "platform/macos/screen_impl.hpp"

namespace MacOS{
struct SXWindowWrapper;
}//namespace MacOS::


@interface SXView: NSView{
    MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height;

@end

@interface SXWindowDelegate: NSObject<NSWindowDelegate>{
    MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(MacOS::SXWindowWrapper*)owner;

-(BOOL)windowShouldClose:(id)sender;

-(void)windowDidResize:(NSNotification*)notification;

@end

@interface SXWindow: NSWindow{
    MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height;

-(void)close;

-(BOOL)canBecomeKeyWindow;

@end

namespace MacOS{

struct SXWindowWrapper{
    SXWindow *Handle;
    SXWindowDelegate *Delegate;
    SXView *View;
    std::queue<Event> EventsQueue;

    Result Open(const ScreenImpl &screen, int width, int height);

    Result Close();

    bool IsOpen()const;

    void SetTitle(const char *title);

    bool PollEvent(Event &event);

    Size2u Size()const;

    void OnWindowClose();

    void OnWindowResized(u32 width, u32 height);

    void OnWindowDraw();
    
    void OnMouseWheel(u32 delta);

    void OnMouseButtonPress(Mouse::Button button, s32 x, s32 y);

    void OnMouseButtonRelease(Mouse::Button button, s32 x, s32 y);

    void OnKeyPress(Key key);

    void OnKeyRelease(Key key);
};

}//namespace MacOS::

#endif//STRAITX_MACOS_SX_WINDOW_HPP
