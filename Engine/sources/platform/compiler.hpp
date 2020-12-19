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



#ifdef NDEBUG
    #define SX_RELEASE
    #define SX_BUILD_TYPE_NAME "Release"
#else
    #define SX_DEBUG
    #define SX_BUILD_TYPE_NAME "Debug"
#endif

// sx_inline will force inline in release mode
#ifdef SX_RELEASE
    #if defined(SX_COMPILER_GCC)
        #define sx_inline __attribute__((always_inline)) inline
    #elif defined(SX_COMPILER_MSVC)
        #define sx_inline __forceinline
    #else
        #warning "Your compiler does not support sx_inline"
        #define sx_inline inline
    #endif
#else
   #define sx_inline inline 
#endif

#endif // STRAITX_COMPILER_HPP