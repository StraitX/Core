#include <cstdlib>
#include <cstdio>
#include <signal.h>
#include "platform/linux/display_server.hpp"
#include "core/span.hpp"
#include "main/guarded_main.hpp"
#include "core/os/stacktrace.hpp"
#include "core/print.hpp"

void CrashHandler(int sig) {
	WriterPrint(STDErrWriter, nullptr, "StraitX Crashed: %\n", Stacktrace(3));

  	signal(sig, SIG_DFL);
	raise(sig);
}

int main(int argc, const char **argv){
	(void)argc;
	(void)argv;

	signal(SIGSEGV, CrashHandler);
	signal(SIGSYS,  CrashHandler);
	signal(SIGTERM, CrashHandler);
	signal(SIGBUS,  CrashHandler);
	signal(SIGABRT, CrashHandler);

	if(!Linux::DisplayServer::Open()){
		fputs("PlatformRuntime: Linux: Can't connect to x11 display server", stderr);
		return EXIT_FAILURE;
	}
	
	X11::XInitThreads();

	int result = GuardedMain(argc, argv);

	Linux::DisplayServer::Close();

	return result;
}