#ifndef STRAITX_TYPES_HPP
#define STRAITX_TYPES_HPP

#include "platform/compiler.hpp"
#include "platform/arch.hpp"
#include <cstddef>

// for now


typedef signed   char s8;
typedef unsigned char u8;

typedef signed   short s16;
typedef unsigned short u16;

typedef signed   int s32;
typedef unsigned int u32;

#if defined(SX_COMPILER_MSVC)
    typedef signed   __int64 s64;
    typedef unsigned __int64 u64;
#else
    typedef signed   long long s64;
    typedef unsigned long long u64;
#endif


typedef std::size_t size_t;

#if defined(SX_ARCH_X86)
    typedef s32 offset_t;
#elif defined(SX_ARCH_X86_64)
    typedef s64 offset_t;
#else
    #error "Your architecture does not support size_t type"
#endif

static_assert(sizeof(size_t) == sizeof(offset_t),"size_t of your arch does not match StraitX requirements");

#if defined(SX_ARCH_X86)
    typedef u32 ptr_t; 
#elif defined(SX_ARCH_X86_64)
    typedef u64 ptr_t;
#else
    #error "Your architecture does not support pointer types"
#endif

typedef decltype(nullptr) nullptr_t;

template <typename T, size_t N>
constexpr size_t lengthof(T (&)[N]){
    return N;
}

struct Point{
    s32 x;
    s32 y;
};
template <typename T>
struct Size2{
    T width;
    T height;
};
typedef Size2<s32> Size2i;
typedef Size2<u32> Size2u;
typedef Size2<float> Size2f;

struct Version{
    u16 Major;
    u16 Minor;
    u32 Patch;
};



#endif // STRAITX_TYPES_HPP