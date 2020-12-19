#ifndef STRAITX_DISPLAY_WIN32_HPP
#define STRAITX_DISPLAY_WIN32_HPP

#include <windows.h>
#include "platform/error.hpp"
#include "platform/compiler.hpp"
#include "platform/windows/screen_win32.hpp"

namespace StraitX {
namespace Windows{

class DisplayWin32 {
private:
public:
    enum class Ext {
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore,
        Count
    };
public:
    DisplayWin32() = default;

    DisplayWin32(const DisplayWin32 & other) = default;

    sx_inline Error Open() {
        return Error::Success;
    }

    sx_inline Error Close() {
        return Error::Success;
    }

    sx_inline bool IsOpened() {
        return true;
    }

    bool CheckSupport(Ext extension);

    ScreenWin32 MainScreen();
};

};// namespace Windows::
};// namespace StraitX::

#endif //STRAITX_DISPLAY_WIN32_HPP