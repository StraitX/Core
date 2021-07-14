#ifndef STRAITX_MACOS_LINEAR_UNITS_H
#define STRAITX_MACOS_LINEAR_UNITS_H

#include <Cocoa/Cocoa.h>

template <typename T>
T PixelsToLinearUnits(const T& pixels, NSScreen *screen){
    return pixels / [screen backingScaleFactor];
}

template <typename T>
T LinearUnitsToPixels(const T& units, NSScreen *screen){
    return units * [screen backingScaleFactor];
}

#endif//STRAITX_MACOS_LINEAR_UNITS_H