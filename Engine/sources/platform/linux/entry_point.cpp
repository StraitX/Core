#include "platform/platform_runtime.hpp"
#include "platform/linux/window_impl.hpp"
#include "platform/clock.hpp"

namespace Linux{
extern WindowImpl s_Window;
extern void PollEvents(const WindowImpl &window, void (*handler)(const Event &e));

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

			Linux::PollEvents(Linux::s_Window, PlatformRuntime::ProcessEvent);

			if(!PlatformRuntime::Tick(dt))
				break;			
			dt = frame_clock.GetElapsedTime().AsSeconds();	
		}
	}

	PlatformRuntime::Finalize();

	return !init;
}