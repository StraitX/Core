#ifndef STRAITX_MEMORY_HPP
#define STRAITX_MEMORY_HPP

#include <new>
#include "platform/types.hpp"
#include "platform/platform_detection.hpp"

#if defined(SX_PLATFORM_LINUX) || defined(SX_PLATFORM_MACOS)
    #include <alloca.h>
#elif defined(SX_PLATFORM_WINDOWS)
    #include <malloc.h>
#else
    #error "Alloca.h on your platform is not supported yet"
#endif

namespace StraitX{

class Memory{
public:
    static void *Alloc(size_t size);

    static void Free(void *pointer);

    static void *Realloc(void *pointer, size_t size);

    static void Set(void *memory, u8 byte, size_t size);

    static void Copy(const void *source, void *destination, size_t size);

    static u64 Allocated();

    static u64 Freed();

    static u64 InUse();

    static u64 AllocCalls();

    static u64 FreeCalls();

};

}; // namespace StraitX::

#endif