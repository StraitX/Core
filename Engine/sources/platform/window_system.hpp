#ifndef STRAITX_WINDOW_SYSTEM_HPP
#define STRAITX_WINDOW_SYSTEM_HPP

#include "platform/platform_detection.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"
#include "platform/result.hpp"

namespace StraitX{

class WindowSystem{
public:
    enum class Ext: u8{
        DoubleBuffer = 0,
        OpenGLLegacy,
        OpenGLCore
    };

    static Result Initialize();

    static Result Finalize();

    static bool CheckSupport(Ext extension);

    static Screen MainScreen();

};

}; // namespace StraitX::

#endif //STRAITX_WINDOW_SYSTEM_HPP