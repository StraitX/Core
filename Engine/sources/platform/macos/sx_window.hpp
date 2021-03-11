#ifndef STRAITX_MACOS_SX_WINDOW_HPP
#define STRAITX_MACOS_SX_WINDOW_HPP

#include "platform/macos/common.hpp"
#import <Cocoa/Cocoa.h>
#include <queue>
#include "platform/result.hpp"
#include "platform/events.hpp"
#include "platform/macos/screen_impl.hpp"

namespace StraitX{
namespace MacOS{
struct SXWindowWrapper;
}//namespace MacOS::
}//namespace StraitX::

@interface SXView: NSView{
    StraitX::MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height;

@end

@interface SXWindowDelegate: NSObject<NSWindowDelegate>{
    StraitX::MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner;

-(BOOL)windowShouldClose:(id)sender;

-(void)windowDidResize:(NSNotification*)notification;

@end

@interface SXWindow: NSWindow{
    StraitX::MacOS::SXWindowWrapper *m_Wrapper;
}

-(instancetype)initWithSXWindow:(StraitX::MacOS::SXWindowWrapper*)owner Width: (int)width Height: (int)height;

-(void)close;

-(void)processEvents;

@end

namespace StraitX{
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

    void OnKeyPress(Keyboard::KeyCode key);

    void OnKeyRelease(Keyboard::KeyCode key);
};

}//namespace MacOS::
}//namespace StraitX::

#endif//STRAITX_MACOS_SX_WINDOW_HPP
