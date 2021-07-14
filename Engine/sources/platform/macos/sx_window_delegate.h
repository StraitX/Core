#ifndef STRAITX_MACOS_SX_WINDOW_DELEGATE_H
#define STRAITX_MACOS_SX_WINDOW_DELEGATE_H

#import <Cocoa/Cocoa.h>
#include "platform/macos/window_impl.hpp"

@interface SXWindowDelegate: NSObject<NSWindowDelegate>{
    MacOS::WindowImpl* WindowImpl;
}

-(instancetype)initWithWindowImpl:(MacOS::WindowImpl*)impl;

-(BOOL)windowShouldClose:(id)sender;

-(void)windowDidResize:(NSNotification*)notification;

@end

#endif//STRAITX_MACOS_SX_WINDOW_DELEGATE_H