#include "platform/platform_runtime.hpp"
#include "platform/linux/window_impl.hpp"

namespace Linux{
extern WindowImpl s_Window;
}//namespace Linux::

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	Result init = PlatformRuntime::Initialize();

	if(init){
		do{
			Event e;
			while(Linux::s_Window.PollEvent(e))
				PlatformRuntime::ProcessEvent(e);
			
		}while(PlatformRuntime::Tick(0.016));
	}

	PlatformRuntime::Finalize();

	return !init;
}