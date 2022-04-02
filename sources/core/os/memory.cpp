#include "core/os/memory.hpp"
#include "core/env/compiler.hpp"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <assert.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

constexpr size_t s_DefaultAlignment =
max(
    max(
        max(
            max(alignof(char), alignof(wchar_t)),
            max(alignof(short), alignof(int))
        ),
        max(
            max(alignof(long), alignof(long long)),
            max(alignof(void*), alignof(ptrdiff_t))
        )
    ),
    max(
        max(
            max(alignof(clock_t), alignof(time_t)),
            max(alignof(size_t), alignof(long double))
        ),
        max(
            max(alignof(float), alignof(double)),
            0
        )
    )
);

#ifndef __STDCPP_DEFAULT_NEW_ALIGNMENT__
#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ s_DefaultAlignment
#endif
static constexpr size_t s_AllocationInfoSize = max(sizeof(size_t), __STDCPP_DEFAULT_NEW_ALIGNMENT__);

void *Memory::Alloc(size_t size){
    return std::malloc(size);
}

void Memory::Free(void *pointer){
    std::free(pointer);
}

void *Memory::Realloc(void *pointer, size_t size){
    return std::realloc(pointer,size);
}

void *Memory::AlignedAlloc(size_t size, size_t alignment){
	if(alignment < __STDCPP_DEFAULT_NEW_ALIGNMENT__)
		alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

	if(alignment != 0 && (alignment & (alignment - 1)) != 0)
		return nullptr;
	return AlignedAllocImpl(size, alignment);
}

void Memory::AlignedFree(void *pointer){
	AlignedFreeImpl(pointer);
}

void Memory::Set(void *memory, u8 byte, size_t size){
    memset(memory,byte,size);
}

void Memory::Copy(const void *source, void *destination, size_t size){
    memcpy(destination,source,size);
}

