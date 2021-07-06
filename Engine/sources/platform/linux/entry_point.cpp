#include "platform/platform_runtime.hpp"
#include "platform/linux/window_impl.hpp"
#include "platform/clock.hpp"

namespace Linux{
extern WindowImpl s_Window;
}//namespace Linux::

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	Result init = PlatformRuntime::Initialize();

	if(init){

		Clock frame_clock;
		float dt = 1.f/60.f;
		for(;;){
			frame_clock.Restart();

			Event e;
			while(Linux::s_Window.PollEvent(e))
				PlatformRuntime::ProcessEvent(e);

			if(!PlatformRuntime::Tick(dt))
				break;			
			dt = frame_clock.GetElapsedTime().AsSeconds();	
		}
	}

	PlatformRuntime::Finalize();

	return !init;
}