#include <X11/Xlib.h>
#include "platform/linux/screen_impl.hpp"

namespace Linux{

ScreenImpl::ScreenImpl(void *screen_handle, unsigned int screen_index){
	Handle = screen_handle;
	Index = screen_index;

    Size.width = XWidthOfScreen((::Screen*)Handle);
    Size.height = XHeightOfScreen((::Screen*)Handle);

    DPI.width  = float(Size.width) / (float(XWidthMMOfScreen((::Screen*)Handle)) / 25.4f);
   	DPI.height = float(Size.height) / (float(XHeightMMOfScreen((::Screen*)Handle)) / 25.4f);
}

} // namespace Linux::