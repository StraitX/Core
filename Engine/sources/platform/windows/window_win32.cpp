#include "platform/windows/window_win32.hpp"
#include "platform/windows/hinstance.hpp"
#include <windows.h>

namespace StraitX{
namespace Windows{

const char* wndClassName = "StraitXWindow";
const char* emptyTitle = "";

WindowWin32::WindowWin32(int width, int height) {
    HINSTANCE instance = HInstance::Instance().Handle();

    mHandle = CreateWindow(wndClassName, emptyTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, instance, 0);
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

void WindowWin32::RegClass() {
    WNDCLASS wndClass = { 0 };
    wndClass.hInstance = HInstance::Instance().Handle();
    wndClass.lpfnWndProc = DefWindowProc;
    wndClass.lpszClassName = wndClassName;
    RegisterClass(&wndClass);
}

bool WindowWin32::PollEvent(Event& event) {
    return false;
}

}; //namespace Windows::
}; //namespace StraitX::