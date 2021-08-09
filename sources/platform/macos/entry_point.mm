#import <Cocoa/Cocoa.h>
#include <cstdlib>
#include <cstdio>
#include "core/os/platform_runtime.hpp"
#include "core/os/clock.hpp"
#include "platform/macos/sx_application.h"
#include "platform/macos/window_impl.hpp"

int main(int argc, char **argv){
	(void)argc;
	(void)argv;

	@autoreleasepool{
	    NSApplication *app = [SXApplication sharedApplication];
	    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
	    [app activateIgnoringOtherApps: YES];

		if(!MacOS::WindowImpl::s_MainWindow.Open(1280, 720)){
			fputs("MacOS: PlatformRuntime: Can't open a window", stderr);
			return EXIT_FAILURE;
		}
		MacOS::WindowImpl::s_MainWindow.EventHandler = PlatformRuntime::HandleEvent;

		Result init = PlatformRuntime::Initialize();

		if(init){
			Clock frame_clock;
			float dt = 1.f/60.f;
			for(;;){
				@autoreleasepool{
					frame_clock.Restart();

					[SXApplication poolEvents];

					if(!PlatformRuntime::Tick(dt))
						break;			
					dt = frame_clock.GetElapsedTime().AsSeconds();	
				}
			}
		}else{
			fputs("MacOS: PlatformRuntime: Initialization failed", stderr);
		}

		PlatformRuntime::Finalize();

		MacOS::WindowImpl::s_MainWindow.Close();

		return !init;
	}

	return 0;
}