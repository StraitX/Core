#import <Cocoa/Cocoa.h>
#include <cstdlib>
#include <cstdio>
#include "main/guarded_main.hpp"
#include "core/os/clock.hpp"
#include "platform/macos/sx_application.h"
#include "platform/macos/window_impl.hpp"

int main(int argc, const char **argv){
	(void)argc;
	(void)argv;

	@autoreleasepool{
	    NSApplication *app = [SXApplication sharedApplication];
	    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
	    [app activateIgnoringOtherApps: YES];

		GuardedMain(argc, argv);
	}

	return 0;
}