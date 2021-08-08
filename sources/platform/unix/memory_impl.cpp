#include "core/os/memory.hpp"
#include <stdlib.h>

void *Memory::AlignedAllocImpl(size_t size, size_t alignment){
	void *pointer = nullptr;
	/*
		On Linux (and other systems), posix_memalign() does
       not modify memptr on failure.  A requirement standardizing this
       behavior was added in POSIX.1-2008 TC2.
	*/
	(void)posix_memalign(&pointer, alignment, size);
	return pointer;
}

void Memory::AlignedFreeImpl(void *pointer){
	free(pointer);
}