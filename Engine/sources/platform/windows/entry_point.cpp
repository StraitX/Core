#include <Windows.h>
#include "platform/windows/window_impl.hpp"
#include "platform/windows/events.hpp"
#include "platform/platform_runtime.hpp"

using namespace Windows;

namespace Windows {
    extern WindowImpl s_Window;
};

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	Result init = PlatformRuntime::Initialize();

	if(init){
        do {
            MSG message = { 0 };
            while (::PeekMessage(&message, (HWND)s_Window.Handle(), 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        } while (PlatformRuntime::Tick(0.016));
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
            PlatformRuntime::ProcessEvent(e);
        
        if(uMsg != WM_CLOSE)
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        return 0;
    }

}//namespace Windows::