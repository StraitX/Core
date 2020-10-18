#ifndef STRAITX_PLATFORM_HPP
#define STRAITX_PLATFORM_HPP

#if defined(_WIN32)

    #define SX_PLATFORM_WINDOWS

#elif defined(__APPLE__) && defined(__MACH__)

    #define SX_PLATFORM_MACOS
    #error Apple operating systems are not supported yet

#elif defined(__unix__)

    #if defined(__ANDROID__)

        #define SX_PLATFORM_ANDROID
        #error Android is not supported yet

    #elif defined(__linux__)

        #define SX_PLATFORM_LINUX

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

        #define SX_PLATFORM_FREEBSD
        #error FreeBSD is not supported yet

    #elif defined(__OpenBSD__)

        #define SX_PLATFORM_OPENBSD
        #error OpenBSD is not supported yet

    #else

        #error This UNIX operating system is not supported

    #endif

#else

    #error This operating system is not supported

#endif

#endif //STRAITX_PLATFORM_HPP