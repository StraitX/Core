#ifndef STRAITX_DISPLAY_HPP
#define STRAITX_DISPLAY_HPP

#include "platform/platform_detection.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"
#include "platform/error.hpp"

#ifdef SX_PLATFORM_LINUX
    #include "platform/linux/display_x11.hpp"
    typedef StraitX::Linux::DisplayX11 DisplayImpl;
#elif defined(SX_PLATFORM_WINDOWS)
    #include "platform/windows/display_win32.hpp"
    typedef StraitX::Windows::DisplayWin32 DisplayImpl;
#else
    #error "Your platform does not support display"
#endif

namespace StraitX{



class Display{
private:
    DisplayImpl m_Impl;
public:

    enum class Ext{
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore
    };

    Error Open();

    Error Close();

    bool IsOpen();

    bool CheckSupport(Ext extension);

    Screen MainScreen();

    DisplayImpl &Impl();
    
    const DisplayImpl &Impl()const;

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_HPP