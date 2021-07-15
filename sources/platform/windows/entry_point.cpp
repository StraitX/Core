#include <Windows.h>
#include <cstdlib>
#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/clock.hpp"
#include "platform/platform_runtime.hpp"

using namespace Windows;

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

    if (!WindowImpl::RegisterWindowClass()) {
        MessageBox(nullptr, "Windows: Can't register a window class", "PlatformRuntime", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    if (!WindowImpl::s_MainWindow.Open(1280, 720)) {
        MessageBox(nullptr, "Windows: Can't open a window", "PlatformRuntime", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    Result init = Result::None;

	if((init = PlatformRuntime::Initialize())){
        float dt = 1.f / 60;
        Clock frame_clock;
        for(;;){
            frame_clock.Restart();
            MSG message = { 0 };
            while (::PeekMessage(&message, (HWND)WindowImpl::s_MainWindow.Handle(), 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            if (!PlatformRuntime::Tick(dt))
                break;
            dt = frame_clock.GetElapsedTime().AsSeconds();
        } 
    }else {
        MessageBox(nullptr, "Windows: Can't initialize PlatformRuntime", "PlatformRuntime", MB_ICONERROR);
    }

	PlatformRuntime::Finalize();

	return !init;
}

namespace Windows {
    //collection of crap to make it works without duplication of event queue
    LRESULT CALLBACK StraitXWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        MSG msg = {};
        msg.message = uMsg;
        msg.hwnd = hwnd;
        msg.lParam = lParam;
        msg.wParam = wParam;
        
        Event e;
        if (ToStraitXEvent(msg, e))
            PlatformRuntime::HandleEvent(e);
        
        if(uMsg != WM_CLOSE)
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        return 0;
    }

}//namespace Windows::