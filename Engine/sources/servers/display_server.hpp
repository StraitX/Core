#ifndef STRAITX_DISPLAY_SERVER_HPP
#define STRAITX_DISPLAY_SERVER_HPP

#include "platform/result.hpp"
#include "graphics/render_window.hpp"

namespace StraitX{

struct DisplayServer{
    static RenderWindow Window;
};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_SERVER_HPP