#ifndef STRAITX_MACOS_SX_APPLICATION_HPP
#define STRAITX_MACOS_SX_APPLICATION_HPP

#import <Cocoa/Cocoa.h>

@interface SXApplication: NSApplication{
}

+(void)poolEvents;

@end

#endif//STRAITX_MACOS_SX_APPLICATION_HPP