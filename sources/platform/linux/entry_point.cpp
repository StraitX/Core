#include <cstdlib>
#include <cstdio>
#include "platform/platform_runtime.hpp"
#include "platform/linux/window_impl.hpp"
#include "platform/linux/display_server.hpp"
#include "platform/clock.hpp"

namespace Linux{
extern void PollEvents(const WindowImpl &window, void (*handler)(const Event &e));
}//namespace Linux::

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	if(!Linux::DisplayServer::Open()){
		fputs("PlatformRuntime: Linux: Can't connect to x11 display server", stderr);
		return EXIT_FAILURE;
	}

	if(!Linux::WindowImpl::s_MainWindow.Open(1280, 720)){
		fputs("PlatformRuntime: Linux: Can't open a window", stderr);
		return EXIT_FAILURE;
	}

	Result init = Result::None;
	if((init = PlatformRuntime::Initialize())){

		Clock frame_clock;
		float dt = 1.f/60.f;
		for(;;){
			frame_clock.Restart();

			Linux::PollEvents(Linux::WindowImpl::s_MainWindow, PlatformRuntime::HandleEvent);

			if(!PlatformRuntime::Tick(dt))
				break;
			dt = frame_clock.GetElapsedTime().AsSeconds();	
		}
	}else{
		fputs("PlatformRuntime: Initialization failed: exiting...", stderr);
	}

	PlatformRuntime::Finalize();

	Linux::WindowImpl::s_MainWindow.Close();

	Linux::DisplayServer::Close();

	return !init;
}