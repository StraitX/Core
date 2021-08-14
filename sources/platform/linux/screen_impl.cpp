#include "platform/linux/screen_impl.hpp"
#include "platform/linux/x11.hpp"

namespace Linux{

ScreenImpl::ScreenImpl(void *screen_handle, unsigned int screen_index){
	Handle = screen_handle;
	Index = screen_index;

    Size.x = X11::XWidthOfScreen((X11::Screen*)Handle);
    Size.y = X11::XHeightOfScreen((X11::Screen*)Handle);

    DPI.x  = float(Size.x) / (float(XWidthMMOfScreen((X11::Screen*)Handle)) / 25.4f);
   	DPI.y = float(Size.y) / (float(XHeightMMOfScreen((X11::Screen*)Handle)) / 25.4f);
}

} // namespace Linux::