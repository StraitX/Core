#ifndef STRAITX_WINDOW_SYSTEM_HPP
#define STRAITX_WINDOW_SYSTEM_HPP

#include "platform/platform_detection.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"
#include "platform/result.hpp"

class WindowSystem{
public:
    static Result Initialize();

    static Result Finalize();

    static PlatformScreen MainScreen();

};

#endif //STRAITX_WINDOW_SYSTEM_HPP