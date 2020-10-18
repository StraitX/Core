#ifndef STRAITX_TYPES_HPP
#define STRAITX_TYPES_HPP

#include "platform/platform.hpp"

#include <cstddef>

// for now

typedef signed   char i8;
typedef unsigned char u8;

typedef signed   short i16;
typedef unsigned short u16;

typedef signed   int i32;
typedef unsigned int u32;

#if defined(_MSC_VER)
    typedef signed   __int64 i64;
    typedef unsigned __int64 u64;
#else
    typedef signed   long long i64;
    typedef unsigned long long u64;
#endif

typedef std::size_t size_t;


#endif // STRAITX_TYPES_HPP