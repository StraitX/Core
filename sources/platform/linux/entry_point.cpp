#include <cstdlib>
#include <cstdio>
#include "core/os/platform_runtime.hpp"
#include "platform/linux/window_impl.hpp"
#include "platform/linux/display_server.hpp"
#include "core/os/clock.hpp"

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

	Result init = Result::None;
	if((init = PlatformRuntime::Initialize())){

		Clock frame_clock;
		float dt = 1.f/60.f;
		for(;;){
			frame_clock.Restart();

			if(!PlatformRuntime::Tick(dt))
				break;
			dt = frame_clock.GetElapsedTime().AsSeconds();	
		}
	}else{
		fputs("PlatformRuntime: Initialization failed: exiting...", stderr);
	}

	PlatformRuntime::Finalize();

	Linux::DisplayServer::Close();

	return !init;
}