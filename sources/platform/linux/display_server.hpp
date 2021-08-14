#ifndef STRAITX_LINUX_DISPLAY_SERVER_HPP
#define STRAITX_LINUX_DISPLAY_SERVER_HPP

#include "core/result.hpp"
#include "platform/linux/x11.hpp"

namespace X11{
struct _XDisplay;
}//namespace X11::

namespace Linux{

struct DisplayServer{
	static X11::_XDisplay *Handle;

	static Result Open();

	static void Close();

	static bool IsOpen();
};

}//namespace Linux::

#endif//STRAITX_LINUX_DISPLAY_SERVER_HPP