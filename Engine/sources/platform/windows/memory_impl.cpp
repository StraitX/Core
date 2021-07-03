#include <Windows.h>
#include "platform/memory.hpp"

void* Memory::AlignedAllocImpl(size_t size, size_t alignment) {
	return _aligned_malloc(size, alignment);
}

void Memory::AlignedFreeImpl(void *pointer) {
	_aligned_free(pointer);
}