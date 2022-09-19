#ifndef STRAITX_NET_BYTE_ORDER_HPP
#define STRAITX_NET_BYTE_ORDER_HPP

#include "core/env/endianness.hpp"
//Assumes all targets are little endian

inline u8 ToNetByteOrder(u8 value) {
    return SwapEndianness(value);
}
inline u16 ToNetByteOrder(u16 value) {
    return SwapEndianness(value);
}
inline u32 ToNetByteOrder(u32 value) {
    return SwapEndianness(value);
}
inline u64 ToNetByteOrder(u64 value) {
    return SwapEndianness(value);
}

inline u8 ToHostByteOrder(u8 value) {
    return SwapEndianness(value);
}
inline u16 ToHostByteOrder(u16 value) {
    return SwapEndianness(value);
}
inline u32 ToHostByteOrder(u32 value) {
    return SwapEndianness(value);
}
inline u64 ToHostByteOrder(u64 value) {
    return SwapEndianness(value);
}

#endif//STRAITX_NET_BYTE_ORDER_HPP