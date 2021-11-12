#include <cstdlib>
#include <cstdio>
#include "platform/linux/display_server.hpp"
#include "core/span.hpp"
#include "main/guarded_main.hpp"

int main(int argc, const char **argv){
	(void)argc;
	(void)argv;

	if(!Linux::DisplayServer::Open()){
		fputs("PlatformRuntime: Linux: Can't connect to x11 display server", stderr);
		return EXIT_FAILURE;
	}

	int result = GuardedMain(argc, argv);

	Linux::DisplayServer::Close();

	return result;
}