#include <windows.h>
#include "platform/windows/window_impl.hpp"
#include "platform/window_system.hpp"

namespace Windows {
    extern LRESULT CALLBACK StraitXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    extern const char* windowClassName;
    WindowImpl s_Window;
    ScreenImpl s_Screen;
}//namespaec Windows::

Result WindowSystem::Initialize(int width, int height) {
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = Windows::StraitXWindowProc;
    windowClass.lpszClassName = Windows::windowClassName;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.style = CS_OWNDC;
    
    if (RegisterClass(&windowClass) == 0)
        return Result::Failure;

    Windows::s_Screen.Size.width = GetSystemMetrics(SM_CXSCREEN);
    Windows::s_Screen.Size.height = GetSystemMetrics(SM_CYSCREEN);

    Windows::s_Screen.DPI.width = 96;
    Windows::s_Screen.DPI.height = 96;

    return Windows::s_Window.Open(Windows::s_Screen, width, height);
}

void WindowSystem::Finalize() {
    Windows::s_Window.Close();
}

PlatformScreen WindowSystem::MainScreen() {
    return Windows::s_Screen;
}

PlatformWindow WindowSystem::Window() {
    return Windows::s_Window;
}