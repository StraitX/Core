#include "platform/windows/window_win32.hpp"
#include "platform/windows/events_win32.hpp"
#include "platform/windows/virtual_keys.hpp"
#include <windows.h>
#include <stdio.h>
namespace StraitX{
namespace Windows{

const char *windowClassName = "StraitXWindow";
const char *windowTitle = "";

WindowWin32::WindowWin32(int width, int height):
    mHandle(nullptr),
    mWidth(width),
    mHeight(height),
    mUnhandledResize(false)
{
    mHandle = CreateWindow(windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr), this);
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
    if (FetchInternalEvents(event)) {
        return true;
    }
    // fetch messages until we catch convertable to StraitX one or message queue become empty
    while (::PeekMessage(&message, (HWND)mHandle, 0, 0, PM_REMOVE)) {
        // okay, WinAPI, lets pretend that we play by your rules
        TranslateMessage(&message);
        DispatchMessage(&message);
        // check if we can convert win32 message to StraitX Event
        if (ToStraitXEvent(message,event)) {        
            return true;
        }
    }
    return false;
}

void WindowWin32::OnResize(int width, int height) {
    mUnhandledResize = true;
    mWidth = width;
    mHeight = height;
}


bool WindowWin32::FetchInternalEvents(Event &event) {
    if (mUnhandledResize){
        event.Type = EventType::WindowResized;
        event.WindowResized.x = mWidth;
        event.WindowResized.y = mHeight;
        mUnhandledResize = false;
        return true;
    }
    return false;
}

//collection of crap to make it works without duplication of event queue
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE) {
        LONG_PTR winPtr = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;
        
        SetWindowLongPtr(hwnd, GWLP_USERDATA, winPtr);
    }
    // assumes that there no other window implementation in the application
    // and we get our own data pointer
    WindowWin32* window = (WindowWin32*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
    case WM_CLOSE:
        PostMessage(hwnd, WM_SX_CLOSE, wParam, lParam);
        break;
    case WM_SIZE:
        window->OnResize(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        PostMessage(hwnd, WM_SX_KEYDOWN, wParam, lParam);
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        PostMessage(hwnd, WM_SX_KEYUP, wParam, lParam);
        break;
    default:  
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void WindowWin32::RegClass() {
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = WindowProc;
    windowClass.lpszClassName = windowClassName;
    windowClass.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
    if (RegisterClass(&windowClass) == 0)
        puts("ERROR: Can't register window class");
}

}; //namespace Windows::
}; //namespace StraitX::