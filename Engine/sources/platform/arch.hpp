#ifndef STRAITX_ARCH_HPP
#define STRAITX_ARCH_HPP


#include "platform/compiler.hpp"

#if defined(SX_COMPILER_GCC) || defined(SX_COMPILER_CLANG)
    #if defined(__i386__)
        #define SX_ARCH_X86
    #elif defined(__x86_64__)
        #define SX_ARCH_X86_64
	#elif defined(__aarch64__)
		#define SX_ARCH_ARM_64
    #else
        #error "Your arch is not supported yet"
    #endif
#elif defined(SX_COMPILER_MSVC)
    #if defined(_M_IX86)
        #define SX_ARCH_X86
    #elif defined(_M_X64)
        #define SX_ARCH_X86_64
    #else
        #error "Your arch is not supported yet"
    #endif
#endif

#ifdef SX_ARCH_X86
    #define SX_ARCH_NAME "x86"
#elif defined(SX_ARCH_X86_64)
    #define SX_ARCH_NAME "x86_64"
#elif defined(SX_ARCH_ARM_64)
	#define SX_ARCH_NAME "arm64"
#endif

#if defined(SX_ARCH_X86)
	#define SX_ARCH_32_BIT
#elif defined(SX_ARCH_X86_64) || defined(SX_ARCH_ARM_64)
	#define SX_ARCH_64_BIT
#endif


#endif // STRAITX_ARCH_HPP