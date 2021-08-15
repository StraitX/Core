#include <Windows.h>
#include <cstdlib>
#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "core/os/clock.hpp"
#include "core/os/platform_runtime.hpp"

using namespace Windows;

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

    if (!WindowImpl::RegisterWindowClass()) {
        MessageBox(nullptr, "Windows: Can't register a window class", "PlatformRuntime", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    Result init = Result::None;

	if((init = PlatformRuntime::Initialize())){
        float dt = 1.f / 60;
        Clock frame_clock;
        for(;;){
            frame_clock.Restart();

            if (!PlatformRuntime::Tick(dt))
                break;
            dt = frame_clock.GetElapsedTime().AsSeconds();
        } 
    }else {
        MessageBox(nullptr, "Windows: Can't initialize PlatformRuntime", "PlatformRuntime", MB_ICONERROR);
    }

	PlatformRuntime::Finalize();

	return !init;
}
