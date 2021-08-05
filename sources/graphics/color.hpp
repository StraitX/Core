#ifndef STRAITX_COLOR_HPP
#define STRAITX_COLOR_HPP

#include "platform/types.hpp"
#include "core/math/vector4.hpp"

struct Color{
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float A = 1.0f;

    constexpr Color() = default;

    constexpr Color(float r, float g, float b, float a = 1.0);

    constexpr Color(u32 rgba8);

    constexpr Color(u8 r, u8 g, u8 b, u8 a = 255);

    constexpr Color(const Color &other) = default;

    constexpr Color& operator=(const Color &other) = default;

	constexpr operator Vector4f()const;

    constexpr u32 RGBA8()const;

    constexpr u32 ABGR8()const;

    //i wish it was a constexpr
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Orange;
    static const Color Mint;
    static const Color LightBlue;
    static const Color Black;
    static const Color White;
};

constexpr Color::Color(float r, float g, float b, float a):
    R(r),
    G(g),
    B(b),
    A(a)
{}

constexpr Color::Color(u32 rgba8):
    Color(
        u8((rgba8 & 0xFF000000) >> 24),
        u8((rgba8 & 0x00FF0000) >> 16), 
        u8((rgba8 & 0x0000FF00) >> 8),
        u8((rgba8 & 0x000000FF) >> 0)
    )
{}

constexpr u32 Color::RGBA8()const{
    u32 color = 0;
    color |= u8(R*u8(-1)) << 24;
    color |= u8(G*u8(-1)) << 16;
    color |= u8(B*u8(-1)) << 8;
    color |= u8(A*u8(-1)) << 0;
    return color;
}

constexpr u32 Color::ABGR8()const {
    u32 color = 0;
    color |= u8(R*u8(-1)) << 0;
    color |= u8(G*u8(-1)) << 8;
    color |= u8(B*u8(-1)) << 16;
    color |= u8(A*u8(-1)) << 24;
    return color;
}

constexpr Color::Color(u8 r, u8 g, u8 b, u8 a):
    R(r/static_cast<float>((decltype(r))-1)),
    G(g/static_cast<float>((decltype(g))-1)),
    B(b/static_cast<float>((decltype(b))-1)),
    A(a/static_cast<float>((decltype(a))-1))
{}
constexpr Color::operator Vector4f()const{
	return {R, G, B, A};
}

constexpr Color operator+(const Color &lvalue, const Color &rvalue){
    return {
        lvalue.R + rvalue.R,
        lvalue.G + rvalue.G,
        lvalue.B + rvalue.B,
        lvalue.A + rvalue.A
    };
}

constexpr Color operator*(const Color &lvalue, const Color &rvalue){
    return {
        lvalue.R * rvalue.R,
        lvalue.G * rvalue.G,
        lvalue.B * rvalue.B,
        lvalue.A * rvalue.A
    };
}

#endif//STRAITX_COLOR_HPP