#ifndef STRAITX_DISPLAY_HPP
#define STRAITX_DISPLAY_HPP

#include "platform/platform_detection.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"
#include "platform/result.hpp"

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

    Display();

    Display(const DisplayImpl &impl);

    Display(const Display &other) = default;

    ~Display() = default;

    enum class Ext{
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore
    };

    Result Open();

    Result Close();

    bool IsOpen();

    bool CheckSupport(Ext extension);

    Screen MainScreen();

    DisplayImpl &Impl();
    
    const DisplayImpl &Impl()const;

};

sx_inline Display::Display():
    m_Impl()
{}

sx_inline Display::Display(const DisplayImpl &impl):
    m_Impl(impl)
{}


sx_inline Result Display::Open(){
    return m_Impl.Open();
}

sx_inline Result Display::Close(){
    return m_Impl.Close();
}

sx_inline bool Display::IsOpen(){
    return m_Impl.IsOpen();
}

sx_inline bool Display::CheckSupport(Ext extension){
    return m_Impl.CheckSupport((DisplayImpl::Ext)extension);
}

sx_inline Screen Display::MainScreen(){
    return m_Impl.MainScreen();
}

sx_inline DisplayImpl &Display::Impl(){
    return m_Impl;
}

sx_inline const DisplayImpl &Display::Impl()const{
    return m_Impl;
}

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_HPP