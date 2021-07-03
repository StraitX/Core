#ifndef STRAITX_DISPLAY_SERVER_HPP
#define STRAITX_DISPLAY_SERVER_HPP

#include "platform/result.hpp"
#include "platform/window.hpp"

namespace StraitX{

struct DisplayServer{
    static PlatformWindow Window;
};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_SERVER_HPP