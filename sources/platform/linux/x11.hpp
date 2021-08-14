#ifndef STRAITX_X11_HPP
#define STRAITX_X11_HPP

namespace X11{

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xlocale.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

#undef KeyPress		
#undef KeyRelease		
#undef ButtonPress		
#undef ButtonRelease		
#undef MotionNotify		
#undef EnterNotify		
#undef LeaveNotify		
#undef FocusIn			
#undef FocusOut		
#undef KeymapNotify		
#undef Expose			
#undef GraphicsExpose		
#undef NoExpose		
#undef VisibilityNotify	
#undef CreateNotify		
#undef DestroyNotify		
#undef UnmapNotify		
#undef MapNotify		
#undef MapRequest		
#undef ReparentNotify		
#undef ConfigureNotify		
#undef ConfigureRequest	
#undef GravityNotify		
#undef ResizeRequest		
#undef CirculateNotify		
#undef CirculateRequest	
#undef PropertyNotify		
#undef SelectionClear		
#undef SelectionRequest	
#undef SelectionNotify		
#undef ColormapNotify		
#undef ClientMessage		
#undef MappingNotify		
#undef GenericEvent		
#undef LASTEvent		

enum EventType{
    KeyPress = 2,
    KeyRelease = 3,
    ButtonPress	= 4,
    ButtonRelease = 5,
    MotionNotify = 6,
    EnterNotify	= 7,
    LeaveNotify	= 8,
    FocusIn = 9,
    FocusOut = 10,
    KeymapNotify = 11,
    Expose = 12,
    GraphicsExpose = 13,
    NoExpose = 14,
    VisibilityNotify = 15,
    CreateNotify = 16,
    DestroyNotify = 17,
    UnmapNotify	= 18,
    MapNotify = 19,
    MapRequest = 20,
    ReparentNotify = 21,
    ConfigureNotify = 22,
    ConfigureRequest = 23,
    GravityNotify = 24,
    ResizeRequest = 25,
    CirculateNotify = 26,
    CirculateRequest = 27,
    PropertyNotify = 28,
    SelectionClear = 29,
    SelectionRequest = 30,
    SelectionNotify = 31,
    ColormapNotify = 32,
    ClientMessage = 33,
    MappingNotify = 34,
    GenericEvent = 35,
    LASTEvent = 36
};

#undef Success
#undef BadRequest
#undef BadValue
#undef BadWindow
#undef BadPixmap
#undef BadAtom
#undef BadCursor
#undef BadFont
#undef BadMatch
#undef BadDrawable
#undef BadAccess 
#undef BadAlloc 
#undef BadColor 
#undef BadGC 
#undef BadIDChoice 
#undef BadName 
#undef BadLength 
#undef BadImplementation 
#undef FirstExtensionError	
#undef LastExtensionError

enum Result{
    Success = 0,
    BadRequest = 1,
    BadValue = 2,
    BadWindow = 3,
    BadPixmap = 4,
    BadAtom = 5,
    BadCursor = 6,
    BadFont = 7,
    BadMatch = 8,
    BadDrawable = 9,
    BadAccess = 10,
    BadAlloc = 11,
    BadColor = 12,
    BadGC = 13,
    BadIDChoice = 14,
    BadName = 15,
    BadLength = 16,
    BadImplementation = 17,
    FirstExtensionError	= 128,
    LastExtensionError	= 255
};

#undef None

enum X{
    None = 0
};

}//namespace X11::

#endif//STRAITX_X11_HPP