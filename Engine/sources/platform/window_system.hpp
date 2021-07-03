#ifndef STRAITX_WINDOW_SYSTEM_HPP
#define STRAITX_WINDOW_SYSTEM_HPP

#include "platform/platform_detection.hpp"
#include "platform/window.hpp"
#include "platform/screen.hpp"
#include "platform/types.hpp"
#include "platform/result.hpp"

class WindowSystem{
private:
	friend class Engine;
    static Result Initialize(int width, int height);

    static void Finalize();
public:
    static PlatformScreen MainScreen();

	static PlatformWindow Window();
};

#endif //STRAITX_WINDOW_SYSTEM_HPP