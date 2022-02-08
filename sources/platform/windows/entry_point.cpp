#include <Windows.h>
#include <cstdlib>
#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "core/os/clock.hpp"
#include "main/guarded_main.hpp"

using namespace Windows;

int main(int argc, char **argv){
	(void)argc;
	(void)argv;
    SetProcessDPIAware();
    if (!WindowImpl::RegisterWindowClass()) {
        MessageBox(nullptr, "Windows: Can't register a window class", "PlatformRuntime", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    int result = GuardedMain(argc, (const char **)argv);

	return result;
}
