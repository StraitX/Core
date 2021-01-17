#ifndef STRAITX_DISPLAY_SERVER_HPP
#define STRAITX_DISPLAY_SERVER_HPP

#include "platform/result.hpp"
#include "platform/window.hpp"

namespace StraitX{

class Engine;

class DisplayServer{
private:
    static DisplayServer *s_DisplayServer;
    Window m_Window;
    Result m_ErrWindow;

    friend class Engine;
public:

    DisplayServer();

    ~DisplayServer();

    Result Initialize();

    Result Finalize();

    static DisplayServer &Instance();

    sx_inline Window &GetWindow(){
        return m_Window;
    }

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_SERVER_HPP