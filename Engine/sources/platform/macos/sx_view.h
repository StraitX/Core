#ifndef STRAITX_MACOS_SX_VIEW_H
#define STRAITX_MACOS_SX_VIEW_H

#import <Cocoa/Cocoa.h>
#include "platform/macos/window_impl.hpp"

@interface SXView: NSView{
    MacOS::WindowImpl *WindowImpl;
}

-(instancetype)initWithWidht: (int)width Height: (int)height WindowImpl: (MacOS::WindowImpl*) impl;

-(void)mouseDown:(NSEvent *)event;

-(void)mouseUp:(NSEvent *)event;

-(void)rightMouseDown:(NSEvent *)event;

-(void)rightMouseUp:(NSEvent *)event;

-(void)otherMouseDown:(NSEvent *)event;

-(void)otherMouseUp:(NSEvent *)event;

-(BOOL)canBecomeKeyView;

-(BOOL)acceptsFirstResponder;

-(BOOL)wantsUpdateLayer;

-(void)keyDown:(NSEvent *)event;

-(void)keyUp:(NSEvent *)event;

-(void)flagsChanged:(NSEvent*)event;

-(void)scrollWheel:(NSEvent *)event;

-(void)mouseDragged:(NSEvent *)event;

-(void)rightMouseDragged:(NSEvent *)event;

-(void)otherMouseDragged:(NSEvent *)event;

//conversion from linear units to pixels happens here
-(void)onPressedButton:(Mouse::Button)button X: (int)x Y: (int)y;

-(void)onReleasedButton:(Mouse::Button)button X: (int)x Y: (int)y;

@end

#endif//STRAITX_MACOS_SX_VIEW_H