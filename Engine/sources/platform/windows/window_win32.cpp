#include "platform/windows/window_win32.hpp"
#include <windows.h>
#include <stdio.h>
namespace StraitX{
namespace Windows{

const char *windowClassName = "StraitXWindow";
const char *windowTitle = "";

WindowWin32::WindowWin32(int width, int height):
    mHandle(nullptr)
{
    mHandle = CreateWindow(windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr), 0);
    ShowWindow((HWND)mHandle, SW_SHOW);
}

WindowWin32::~WindowWin32() {
    DestroyWindow((HWND)mHandle);
}

void WindowWin32::SetTitle(const char* title) {
    SetWindowText((HWND)mHandle, title);
}

void WindowWin32::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetWindowRect((HWND)mHandle, &currentWindowDimens);
    
    SetWindowPos((HWND)mHandle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, SWP_ASYNCWINDOWPOS);
}

bool WindowWin32::PollEvent(Event& event) {
    MSG message = { 0 };
    if (::PeekMessage(&message, (HWND)mHandle, 0, 0, PM_REMOVE)) {
        DispatchMessage(&message);
    }
    return false;
}

void WindowWin32::RegClass() {
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = DefWindowProc;
    windowClass.lpszClassName = windowClassName;
    windowClass.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
    if (RegisterClass(&windowClass) == 0)
        puts("ERROR: Can't register window class");
}

}; //namespace Windows::
}; //namespace StraitX::