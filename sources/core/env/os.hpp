#ifndef STRAITX_OS_DETECTION_HPP
#define STRAITX_OS_DETECTION_HPP

#if defined(_WIN32)

    #define SX_OS_WINDOWS
    #define SX_OS_NAME "Windows"

#elif defined(__APPLE__) && defined(__MACH__)

    #define SX_OS_MACOS
    #define SX_OS_NAME "MacOS"

#elif defined(__unix__)

    #if defined(__ANDROID__)

        #define SX_OS_ANDROID
        #define SX_OS_NAME "Android"
        #error Android is not supported yet

    #elif defined(__linux__)

        #define SX_OS_LINUX
        #define SX_OS_NAME "Linux"

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

        #define SX_OS_FREEBSD
        #define SX_OS_NAME "FreeBSD"
        #error FreeBSD is not supported yet

    #elif defined(__OpenBSD__)

        #define SX_OS_OPENBSD
        #define SX_OS_NAME "OpenBSD"
        #error OpenBSD is not supported yet

    #else

        #error This UNIX operating system is not supported

    #endif

#else

    #error This operating system is not supported

#endif

#endif //STRAITX_OS_DETECTION_HPP