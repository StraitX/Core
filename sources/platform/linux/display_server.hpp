#ifndef STRAITX_LINUX_DISPLAY_SERVER_HPP
#define STRAITX_LINUX_DISPLAY_SERVER_HPP

#include "core/result.hpp"
#include "platform/linux/x11.hpp"

namespace X11{
struct _XDisplay;
}//namespace X11::

namespace Linux{

class DisplayServerClient{
public:
	DisplayServerClient();

	~DisplayServerClient();

	X11::_XDisplay *GetX11ServerHandle()const;

	static void ForceInit();
};

}//namespace Linux::

#endif//STRAITX_LINUX_DISPLAY_SERVER_HPP