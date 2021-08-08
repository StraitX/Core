#ifndef STRAITX_LINUX_DISPLAY_SERVER_HPP
#define STRAITX_LINUX_DISPLAY_SERVER_HPP

struct _XDisplay;

#undef Success
#undef None
#include "core/result.hpp"

namespace Linux{

struct DisplayServer{
	static _XDisplay *Handle;

	static Result Open();

	static void Close();

	static bool IsOpen();
};

}//namespace Linux::

#endif//STRAITX_LINUX_DISPLAY_SERVER_HPP