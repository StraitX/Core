#ifndef STRAITX_DISPLAY_SERVER_HPP
#define STRAITX_DISPLAY_SERVER_HPP

#include "platform/error.hpp"
#include "platform/display.hpp"
#include "platform/window.hpp"

namespace StraitX{

class Engine;

class DisplayServer{
private:
    static DisplayServer *s_DisplayServer;
    Display m_Display;
    Window m_Window;
    Error m_ErrDisplay, m_ErrWindow;

    friend class Engine;
public:

    DisplayServer();

    ~DisplayServer();

    Error Initialize(const PixelFormat &format, const Display::Ext api);

    Error Finalize();

    static DisplayServer &Instance();

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_SERVER_HPP