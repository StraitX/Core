#ifndef STRAITX_DISPLAY_HPP
#define STRAITX_DISPLAY_HPP

#include "platform/platform_detection.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/display_x11.hpp"
    typedef StraitX::Linux::DisplayX11 DisplayImpl;
#else
    #error "Your platform does not support display"
#endif

namespace StraitX{

class Display{
private:
    DisplayImpl m_Impl;

    static Display s_Instance;
public:

    Error Open();

    Error Close();

    bool IsOpen();

    Screen MainScreen();

    DisplayImpl &Impl();
    
    const DisplayImpl &Impl()const;

    static Display &Instance();

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_HPP