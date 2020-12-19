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

    sx_inline Display():
        m_Impl()
    {}

    sx_inline Display(const DisplayImpl &impl):
        m_Impl(impl)
    {}

    Display(const Display &other) = default;

    ~Display() = default;

    enum class Ext{
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore
    };

    sx_inline Error Open(){
        return m_Impl.Open();
    }

    sx_inline Error Close(){
        return m_Impl.Close();
    }

    sx_inline bool IsOpen(){
        return m_Impl.IsOpen();
    }

    sx_inline bool CheckSupport(Ext extension){
        return m_Impl.CheckSupport((DisplayImpl::Ext)extension);
    }

    sx_inline Screen MainScreen(){
        return m_Impl.MainScreen();
    }

    sx_inline DisplayImpl &Impl(){
        return m_Impl;
    }
    
    sx_inline const DisplayImpl &Impl()const{
        return m_Impl;
    }

};

}; // namespace StraitX::

#endif // STRAITX_DISPLAY_HPP