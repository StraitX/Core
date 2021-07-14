#ifndef STRAITX_MACOS_SX_WINDOW_H
#define STRAITX_MACOS_SX_WINDOW_H

#import <Cocoa/Cocoa.h>

@interface SXWindow: NSWindow{
}

-(instancetype)initWithWidth: (int)width Height: (int)height;

-(void)dealloc;

-(BOOL)canBecomeKeyWindow;

-(BOOL)canBecomeMainWindow;

@end

#endif//STRAITX_MACOS_SX_WINDOW_H