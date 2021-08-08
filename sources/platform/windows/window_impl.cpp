#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/windows/virtual_keys.hpp"
#include <windows.h>

namespace Windows{

extern LRESULT CALLBACK StraitXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
const char *s_WindowClassName = "StraitXWindow";
const char *s_WindowTitle = "";
DWORD s_WindowStyle = WS_OVERLAPPEDWINDOW;

WindowImpl WindowImpl::s_MainWindow;

Result WindowImpl::Open(int width, int height) {
    RECT dimensions = { 0, 0, width, height};

    AdjustWindowRect(&dimensions, s_WindowStyle, false);

    //Warning: width and height now are used for better purpose
    width = dimensions.right - dimensions.left;
    height = dimensions.bottom - dimensions.top;

    m_Handle = CreateWindow(s_WindowClassName, s_WindowTitle, s_WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr), nullptr);
    ShowWindow(m_Handle, SW_SHOW);
    return ResultError(m_Handle == nullptr);
}

Result WindowImpl::Close() {
    return ResultError(DestroyWindow(m_Handle) == 0);
}

void WindowImpl::SetTitle(const char* title) {
    (void)SetWindowText(m_Handle, title);
}

Vector2u WindowImpl::Size()const{
    return GetSizeFromHandle(m_Handle);
}

void WindowImpl::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetClientRect(m_Handle, &currentWindowDimens);

    SetWindowPos(m_Handle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, 0);
}

const PlatformScreen& WindowImpl::Screen(){
    //TODO: For now windows does not support multiple screens 
    if (m_Screen.Handle == nullptr) {
        m_Screen.Handle = (void*)0xAA;
        m_Screen.Size.x = GetSystemMetrics(SM_CXSCREEN);
        m_Screen.Size.y = GetSystemMetrics(SM_CYSCREEN);
        
        float dpi = GetDpiForWindow(m_Handle);
        m_Screen.DPI.x = dpi;
        m_Screen.DPI.y = dpi;
    }
    return m_Screen;
}


Vector2u WindowImpl::GetSizeFromHandle(HWND__* handle) {
    RECT currentWindowDimens = { 0 };
    GetClientRect(handle, &currentWindowDimens);

    return {u32(currentWindowDimens.right - currentWindowDimens.left), u32(currentWindowDimens.bottom - currentWindowDimens.top)};
}

Result WindowImpl::RegisterWindowClass() {
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = StraitXWindowProc;
    windowClass.lpszClassName = s_WindowClassName;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.style = CS_OWNDC;
    
    return Result(RegisterClass(&windowClass) != 0);
}

}//namespace Windows::