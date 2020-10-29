#ifndef STRAITX_PLATFORM_DETECTION_HPP
#define STRAITX_PLATFORM_DETECTION_HPP

#if defined(_WIN32)

    #define SX_PLATFORM_WINDOWS
    #define SX_PLATFORM_NAME "Windows"

#elif defined(__APPLE__) && defined(__MACH__)

    #define SX_PLATFORM_MACOS
    #define SX_PLATFORM_NAME "MacOS"
    #error Apple operating systems are not supported yet

#elif defined(__unix__)

    #if defined(__ANDROID__)

        #define SX_PLATFORM_ANDROID
        #define SX_PLATFORM_NAME "Android"
        #error Android is not supported yet

    #elif defined(__linux__)

        #define SX_PLATFORM_LINUX
        #define SX_PLATFORM_NAME "Linux"

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

        #define SX_PLATFORM_FREEBSD
        #define SX_PLATFORM_NAME "FreeBSD"
        #error FreeBSD is not supported yet

    #elif defined(__OpenBSD__)

        #define SX_PLATFORM_OPENBSD
        #define SX_PLATFORM_NAME "OpenBSD"
        #error OpenBSD is not supported yet

    #else

        #error This UNIX operating system is not supported

    #endif

#else

    #error This operating system is not supported

#endif

#endif //STRAITX_PLATFORM_DETECTION_HPP