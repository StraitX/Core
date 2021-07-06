#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/windows/virtual_keys.hpp"
#include <windows.h>

namespace Windows{

const char *windowClassName = "StraitXWindow";
const char *windowTitle = "";
DWORD style = WS_OVERLAPPEDWINDOW;

Result WindowImpl::Open(const ScreenImpl& screen, int width, int height) {
    RECT dimensions = { 0, 0, width, height};

    AdjustWindowRect(&dimensions, style, false);

    //Warning: width and height now are used for better purpose
    width = dimensions.right - dimensions.left;
    height = dimensions.bottom - dimensions.top;

    m_Handle = CreateWindow(windowClassName, windowTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr), nullptr);
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

Size2u WindowImpl::Size()const{
    return GetSizeFromHandle(m_Handle);
}

void WindowImpl::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetClientRect(m_Handle, &currentWindowDimens);

    SetWindowPos(m_Handle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, 0);
}


Size2u WindowImpl::GetSizeFromHandle(HWND__* handle) {
    RECT currentWindowDimens = { 0 };
    GetClientRect(handle, &currentWindowDimens);

    return {u32(currentWindowDimens.right - currentWindowDimens.left), u32(currentWindowDimens.bottom - currentWindowDimens.top)};
}

}//namespace Windows::