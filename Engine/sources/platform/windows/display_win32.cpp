#include "platform/windows/display_win32.hpp"
#include <windows.h>

namespace StraitX {
namespace Windows{

extern const char* windowClassName;

Result DisplayWin32::Open() {
	m_HInstance = (HINSTANCE)GetModuleHandle();
	
	if (m_HInstance == nullptr)
		return Result::Failure;

    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = WindowProc;
    windowClass.lpszClassName = windowClassName;
    windowClass.hInstance = m_HInstance;

	return ResultError(RegisterClass(&windowClass) == 0);
}

	
Result DisplayWin32::Close() {
	m_HInstance = nullptr;
	return ResultError(UnregisterClass(&windowClass) == 0);
}


ScreenWin32 DisplayWin32::MainScreen() {
	
	ScreenWin32 result;

	return result;
}
};// namespace Windows::
};// namespace StraitX::