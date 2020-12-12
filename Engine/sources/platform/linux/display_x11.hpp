#ifndef STRAITX_LINUX_DISPLAY_HPP
#define STRAITX_LINUX_DISPLAY_HPP

#include "platform/error.hpp"
#include "platform/linux/screen_x11.hpp"

struct _XDisplay;

namespace StraitX{
namespace Linux{

class DisplayX11{
private:
    ::_XDisplay *m_Handle = nullptr;
public:
    enum class Ext{
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore,
        Count
    };
public:
    DisplayX11() = default;

    DisplayX11(const DisplayX11 &other) = default;

    Error Open();

    Error Close();

    bool IsOpened();

    bool CheckSupport(Ext extension);

    ScreenX11 MainScreen();

    ::_XDisplay *Handle()const;
private:
    bool CheckX11Extension(const char *name);

    bool CheckGLXExtension(const char *name);
};

};// namespace Linux::
};// namespace StraitX::

#endif