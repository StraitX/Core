#ifndef STRAITX_DISPLAY_SERVER_HPP
#define STRAITX_DISPLAY_SERVER_HPP

#include "platform/result.hpp"
#include "platform/display.hpp"
#include "platform/window.hpp"

namespace StraitX{

class Engine;

class DisplayServer{
private:
    static DisplayServer *s_DisplayServer;
    Display m_Display;
    Window m_Window;
    Result m_ErrDisplay, m_ErrWindow;

    friend class Engine;
public:

    DisplayServer();

    ~DisplayServer();

    Result Initialize(const PixelFormat &format);

    Result Finalize();

    static DisplayServer &Instance();

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_SERVER_HPP