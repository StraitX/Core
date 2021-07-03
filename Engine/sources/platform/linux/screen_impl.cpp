#include <X11/Xlib.h>
#include "platform/linux/screen_impl.hpp"

namespace Linux{

ScreenImpl::ScreenImpl(void *display_handle){
	Handle = DefaultScreenOfDisplay((::Display*)display_handle);

	Index = DefaultScreen((::Display*)display_handle);

    Size.width = XWidthOfScreen((::Screen*)Handle);
    Size.height = XHeightOfScreen((::Screen*)Handle);

    DPI.width  = float(Size.width) / (float(XWidthMMOfScreen((::Screen*)Handle)) / 25.4f);
   	DPI.height = float(Size.height) / (float(XHeightMMOfScreen((::Screen*)Handle)) / 25.4f);
}

} // namespace Linux::