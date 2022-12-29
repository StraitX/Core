#ifndef STRAITX_MEMORY_HPP
#define STRAITX_MEMORY_HPP

#include <new>
#include "core/types.hpp"
#include "core/env/os.hpp"

#if defined(SX_OS_LINUX) || defined(SX_OS_MACOS)
    #include <alloca.h>
#elif defined(SX_OS_WINDOWS)
    #include <malloc.h>
#else
    #error "Alloca.h on your platform is not supported yet"
#endif


#define SX_STACK_ALLOC(size) alloca(size)
#define SX_STACK_ARRAY_ALLOC(type, count) (type*)SX_STACK_ALLOC(sizeof(type)*(count))

class Memory{
public:
    static constexpr size_t Byte = 1;
    static constexpr size_t Kilobyte = Byte * 1024;
    static constexpr size_t Megabyte = Kilobyte * 1024;
    static constexpr size_t Gigabyte = Megabyte * 1024;
    static constexpr size_t Terabyte = Gigabyte * 1024;
    static constexpr size_t Petabyte = Terabyte * 1024;
public:
    static void *Alloc(size_t size);

    static void Free(void *pointer);
	
    static void *Realloc(void *pointer, size_t size);

	//alignment should be power of 2
	static void *AlignedAlloc(size_t size, size_t alignment);

	static void AlignedFree(void *pointer);

    static void Set(void *memory, u8 byte, size_t size);

    static void Copy(const void *source, void *destination, size_t size);
private:
	// Implemented per platform
	static void *AlignedAllocImpl(size_t size, size_t alignment);

	static void AlignedFreeImpl(void *pointer);
};

#endif