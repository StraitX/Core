#ifndef STRAITX_MACOS_SX_WINDOW_H
#define STRAITX_MACOS_SX_WINDOW_H

#import <Cocoa/Cocoa.h>
#include "platform/macos/window_impl.hpp"

@interface SXWindow: NSWindow{
    MacOS::WindowImpl *WindowImpl;
}

-(instancetype)initWithWidth: (int)width Height: (int)height WindowImpl: (MacOS::WindowImpl*)impl;

-(void)dealloc;

-(BOOL)canBecomeKeyWindow;

-(BOOL)canBecomeMainWindow;

-(void)becomeKeyWindow;

-(void)resignKeyWindow;

@end

#endif//STRAITX_MACOS_SX_WINDOW_H