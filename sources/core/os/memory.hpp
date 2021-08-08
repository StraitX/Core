#ifndef STRAITX_MEMORY_HPP
#define STRAITX_MEMORY_HPP

#include <new>
#include "core/types.hpp"
#include "core/env/os.hpp"

#if defined(SX_PLATFORM_LINUX) || defined(SX_PLATFORM_MACOS)
    #include <alloca.h>
#elif defined(SX_PLATFORM_WINDOWS)
    #include <malloc.h>
#else
    #error "Alloca.h on your platform is not supported yet"
#endif


#define SX_STACK_ALLOC(size) alloca(size)
#define SX_STACK_ARRAY_ALLOC(type, count) (type*)SX_STACK_ALLOC(sizeof(type)*count)

class Memory{
public:
    static void *Alloc(size_t size);

    static void Free(void *pointer);
	
    static void *Realloc(void *pointer, size_t size);

	//alignment should be power of 2
	static void *AlignedAlloc(size_t size, size_t alignment);

	static void AlignedFree(void *pointer);

    static void Set(void *memory, u8 byte, size_t size);

    static void Copy(const void *source, void *destination, size_t size);

    static u64 Allocated();

    static u64 Freed();

    static u64 InUse();

    static u64 AllocCalls();

    static u64 FreeCalls();
private:
	// Implemented per platform
	static void *AlignedAllocImpl(size_t size, size_t alignment);

	static void AlignedFreeImpl(void *pointer);
};

#endif