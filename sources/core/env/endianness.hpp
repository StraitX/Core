#ifndef STRAITX_ENDIANNESS_HPP
#define STRAITX_ENDIANNESS_HPP

#include "core/types.hpp"

constexpr bool IsLittleEndian() {
    constexpr u16 U16 = 0x0102;
    constexpr u8 U8 = (const u8 &)U16;
    return U8 == 0x02;
}

static_assert(IsLittleEndian(), "StraitX assumes little endian machine");

constexpr u8 SwapEndianness(u8 value) {
    return value;
}

constexpr u16 SwapEndianness(u16 value) {
    return (value >> 8) | (value << 8);
}

constexpr u32 SwapEndianness(u32 value) {
    u32 tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
    return (tmp << 16) | (tmp >> 16);
}

constexpr u64 SwapEndianness(u64 value) {
    return ((value & 0x00000000FFFFFFFFull) << 32) | ((value & 0xFFFFFFFF00000000ull) >> 32)
        |  ((value & 0x0000FFFF0000FFFFull) << 16) | ((value & 0xFFFF0000FFFF0000ull) >> 16)
        |  ((value & 0x00FF00FF00FF00FFull) << 8)  | ((value & 0xFF00FF00FF00FF00ull) >> 8 );
}

#endif//STRAITX_ENDIANNESS_HPP