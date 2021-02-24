#include <windows.h>
#include "platform/window_system.hpp"

namespace StraitX {
namespace Windows {
    extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    extern const char* windowClassName;
}//namespaec Windows::

Result WindowSystem::Initialize() {
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = Windows::WindowProc;
    windowClass.lpszClassName = Windows::windowClassName;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.style = CS_OWNDC;

	return ResultError(RegisterClass(&windowClass) == 0);
}

Result WindowSystem::Finalize() {
    return Result::Success;
}

Screen WindowSystem::MainScreen() {
    return Windows::ScreenImpl();
}

}//namespace StraitX::