#ifndef STRAITX_COMPILER_HPP
#define STRAITX_COMPILER_HPP

// Identify compiler
// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html



#if defined(__clang__)
    #define SX_COMPILER_CLANG
    #define SX_COMPILER_NAME "clang"
#elif defined(__GNUC__)
    #define SX_COMPILER_GCC
    #define SX_COMPILER_NAME "gcc"
#elif defined(_MSC_VER)
    #define SX_COMPILER_MSVC
    #define SX_COMPILER_NAME "msvc"
#elif defined(__MINGW32__) || defined(__MINGW64__)
    #error MinGW compiler is not supported yet
#else
    #error Your compiler is not supported yet
#endif

// should be supported by all compilers due to c standard
#define SX_THIS_FILE __FILE__
#define SX_THIS_LINE __LINE__


#ifdef SX_COMPILER_MSVC
    #define SX_THIS_FUNCTION __FUNCSIG__
#elif defined(SX_COMPILER_GCC) || defined(SX_COMPILER_CLANG)
    #define SX_THIS_FUNCTION __PRETTY_FUNCTION__
#else
    #error Your compiler is not supported yet
#endif


#endif // STRAITX_COMPILER_HPP