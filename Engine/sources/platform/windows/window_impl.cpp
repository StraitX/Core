#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/windows/virtual_keys.hpp"
#include <windows.h>

namespace StraitX{
namespace Windows{

const char *windowClassName = "StraitXWindow";
const char *windowTitle = "";

Result WindowImpl::Open(const ScreenImpl& screen, int width, int height) {
    m_Width = width;
    m_Height = height;
    m_Handle = CreateWindow(windowClassName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr),this);
    ShowWindow(m_Handle, SW_SHOW);
    return ResultError(m_Handle == nullptr);
}

Result WindowImpl::Close() {
    return ResultError(DestroyWindow(m_Handle) == 0);
}

bool WindowImpl::IsOpen()const{
    return m_Handle;
}

void WindowImpl::SetTitle(const char* title) {
    (void)SetWindowText(m_Handle, title);
}

void WindowImpl::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetWindowRect(m_Handle, &currentWindowDimens);
    
    SetWindowPos(m_Handle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, SWP_ASYNCWINDOWPOS);
}

Size2u WindowImpl::Size()const{
    RECT currentWindowDimens = { 0 };
    GetWindowRect(m_Handle, &currentWindowDimens);
    // TODO: Make sure it works
    return {u32(currentWindowDimens.right - currentWindowDimens.left), u32(currentWindowDimens.bottom - currentWindowDimens.top)};
}

bool WindowImpl::PollEvent(Event& event) {
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

void WindowImpl::OnResize(int width, int height) {
    m_UnhandledResize = true;
    m_Width = width;
    m_Height = height;
}


bool WindowImpl::FetchInternalEvents(Event &event) {
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
    WindowImpl* window = (WindowImpl*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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