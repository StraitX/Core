#include "platform/windows/window_win32.hpp"
#include "platform/windows/events_win32.hpp"
#include "platform/windows/virtual_keys.hpp"
#include <windows.h>
#include <stdio.h>
namespace StraitX{
namespace Windows{

const char *windowClassName = "StraitXWindow";
const char *windowTitle = "";

WindowWin32::WindowWin32(DisplayWin32& display) :
    m_Display(display),
    m_Handle(nullptr),
    m_Width(0),
    m_Height(0),
    m_UnhandledResize(false)
{

}

Result WindowWin32::Open(const ScreenWin32& screen, int width, int height) {
    m_Width = width;
    m_Height = height;
    m_Handle = CreateWindow(windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, 0, 0, (HINSTANCE)m_Display.Handle(), this);
    ShowWindow((HWND)m_Handle, SW_SHOW);
    return ResultError(m_Handle == nullptr);
}

Result WindowWin32::Close() {
    return ResultError(DestroyWindow((HWND)m_Handle) == 0);
}

bool WindowWin32::IsOpen()const{
    return m_Handle;
}

void WindowWin32::SetTitle(const char* title) {
    (void)SetWindowText((HWND)m_Handle, title);
}

void WindowWin32::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetWindowRect((HWND)m_Handle, &currentWindowDimens);
    
    SetWindowPos((HWND)m_Handle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, SWP_ASYNCWINDOWPOS);
}
bool WindowWin32::PollEvent(Event& event) {
    MSG message = { 0 };
    if (FetchInternalEvents(event)) {
        return true;
    }
    // fetch messages until we catch convertable to StraitX one or message queue become empty
    while (::PeekMessage(&message, (HWND)m_Handle, 0, 0, PM_REMOVE)) {
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
    m_UnhandledResize = true;
    m_Width = width;
    m_Height = height;
}


bool WindowWin32::FetchInternalEvents(Event &event) {
    if (m_UnhandledResize){
        event.Type = EventType::WindowResized;
        event.WindowResized.x = m_Width;
        event.WindowResized.y = m_Height;
        m_UnhandledResize = false;
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



}; //namespace Windows::
}; //namespace StraitX::