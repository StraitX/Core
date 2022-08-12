#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/windows/virtual_keys.hpp"
#include "core/log.hpp"
#include <windows.h>

namespace Windows{

LRESULT CALLBACK StraitXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static const auto& GetWindowClass() {
    struct WindowClass {
        const char* const Name = "StraitXWindow";
        DWORD Style = WS_OVERLAPPEDWINDOW;
        Result CreationResult = Result::None;

        WindowClass() {
            SetProcessDPIAware();

            WNDCLASS windowClass = { 0 };
            windowClass.lpfnWndProc = StraitXWindowProc;
            windowClass.lpszClassName = Name;
            windowClass.hInstance = GetModuleHandle(nullptr);
            windowClass.style = CS_OWNDC;
    
            CreationResult = Result(RegisterClass(&windowClass) != 0);
        }
    };

    static WindowClass s_WindowClass;
    
    return s_WindowClass;
}


Result WindowImpl::Open(int width, int height, StringView title) {
    if (!GetWindowClass().CreationResult)
        return (LogError("Can't create WindowClass"), Result::Failure);

    RECT dimensions = { 0, 0, width, height};

    AdjustWindowRect(&dimensions, GetWindowClass().Style, false);

    //Warning: width and height now are used for better purpose
    width = dimensions.right - dimensions.left;
    height = dimensions.bottom - dimensions.top;

    m_Handle = CreateWindow(GetWindowClass().Name, title.Data(), GetWindowClass().Style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, (HINSTANCE)GetModuleHandle(nullptr), this);
    ShowWindow(m_Handle, SW_SHOW);

    return Result(m_Handle != nullptr);
}

Result WindowImpl::Close() {
    auto res = DestroyWindow(m_Handle);

    m_Handle = nullptr;

    return Result(res != 0);
}

bool WindowImpl::IsOpen()const {
    return m_Handle != nullptr;
}

void WindowImpl::SetEventsHandler(Function<void(const Event& e)> handler) {
    m_EventsHandler = handler;
}

LRESULT CALLBACK StraitXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (uMsg == WM_CREATE){
        LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

        SetWindowLongPtrA(hwnd, GWLP_USERDATA, window);
    }

    WindowImpl* window = hwnd ? reinterpret_cast<WindowImpl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) : nullptr;
    
    if (window) {
        MSG msg = {};
        msg.message = uMsg;
        msg.hwnd = hwnd;
        msg.lParam = lParam;
        msg.wParam = wParam;

        Event e;
        if (ToStraitXEvent(msg, e))
            window->EventsHandler().TryCall(e);
    }

    if (uMsg != WM_CLOSE)
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    return 0;
}

void WindowImpl::DispatchEvents() {
    MSG message = { 0 };
	while (::PeekMessage(&message, (HWND)m_Handle, 0, 0, PM_REMOVE)) {
	    TranslateMessage(&message);
	    DispatchMessage(&message);
	}
}

void WindowImpl::SetTitle(StringView title) {
    (void)SetWindowText(m_Handle, title.Data());
}

Vector2u WindowImpl::Size()const{
    RECT currentWindowDimens = { 0 };
    GetClientRect(m_Handle, &currentWindowDimens);

    return {u32(currentWindowDimens.right - currentWindowDimens.left), u32(currentWindowDimens.bottom - currentWindowDimens.top)};
}

void WindowImpl::SetSize(int width, int height) {
    RECT currentWindowDimens = { 0 };
    GetClientRect(m_Handle, &currentWindowDimens);

    SetWindowPos(m_Handle, HWND_TOP, currentWindowDimens.left, currentWindowDimens.top, width, height, 0);
}

const Screen& WindowImpl::CurrentScreen()const{
    //TODO: For now windows does not support multiple screens 
    if (m_CurrentScreen.Handle == nullptr) {
        m_CurrentScreen.Handle = (void*)0xAA;
        m_CurrentScreen.Size.x = GetSystemMetrics(SM_CXSCREEN);
        m_CurrentScreen.Size.y = GetSystemMetrics(SM_CYSCREEN);
        
        float dpi = GetDpiForWindow(m_Handle);
        m_CurrentScreen.DPI.x = dpi;
        m_CurrentScreen.DPI.y = dpi;
    }
    return m_CurrentScreen;
}

}//namespace Windows::