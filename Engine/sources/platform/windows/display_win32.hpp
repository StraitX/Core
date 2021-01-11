#ifndef STRAITX_DISPLAY_WIN32_HPP
#define STRAITX_DISPLAY_WIN32_HPP

#include <windows.h>
#include "platform/result.hpp"
#include "platform/compiler.hpp"
#include "platform/windows/screen_win32.hpp"

namespace StraitX {
namespace Windows{

class DisplayWin32 {
private:
    void *m_HInstance = nullptr;
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

    Result Open();

    Result Close();

    bool IsOpen() {
        return m_HInstance;
    }

    sx_inline bool IsOpened() {
        return m_HInstance != nullptr;
    }

    bool CheckSupport(Ext extension);

    ScreenWin32 MainScreen();

    void* Handle() {
        return m_HInstance;
    }
};

};// namespace Windows::
};// namespace StraitX::

#endif //STRAITX_DISPLAY_WIN32_HPP