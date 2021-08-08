#include <X11/Xlib.h>
#include "platform/linux/screen_impl.hpp"

namespace Linux{

ScreenImpl::ScreenImpl(void *screen_handle, unsigned int screen_index){
	Handle = screen_handle;
	Index = screen_index;

    Size.x = XWidthOfScreen((::Screen*)Handle);
    Size.y = XHeightOfScreen((::Screen*)Handle);

    DPI.x  = float(Size.x) / (float(XWidthMMOfScreen((::Screen*)Handle)) / 25.4f);
   	DPI.y = float(Size.y) / (float(XHeightMMOfScreen((::Screen*)Handle)) / 25.4f);
}

} // namespace Linux::