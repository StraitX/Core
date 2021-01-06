#ifndef STRAITX_TYPES_HPP
#define STRAITX_TYPES_HPP

#include "platform/compiler.hpp"
#include "platform/arch.hpp"
#include <cstddef>

// for now

typedef signed   char i8;
typedef unsigned char u8;

typedef signed   short i16;
typedef unsigned short u16;

typedef signed   int i32;
typedef unsigned int u32;

#if defined(SX_COMPILER_MSVC)
    typedef signed   __int64 i64;
    typedef unsigned __int64 u64;
#else
    typedef signed   long long i64;
    typedef unsigned long long u64;
#endif

#if defined(SX_ARCH_X86)
    typedef u32 ptr_t; 
#elif defined(SX_ARCH_X86_64)
    typedef u64 ptr_t;
#else
    #error "Your architecture does not support pointer types"
#endif

typedef std::size_t size_t;

typedef decltype(nullptr) nullptr_t;

struct Point{
    i32 x;
    i32 y;
};
template <typename T>
struct Size2{
    T width;
    T height;
};
typedef Size2<int> Size2i;
typedef Size2<float> Size2f;

struct Version{
    u16 Major;
    u16 Minor;
    u32 Patch;
};

#endif // STRAITX_TYPES_HPP