#ifndef STRAITX_OPENGL_HPP
#define STRAITX_OPENGL_HPP

#include "platform/platform_detection.hpp"

#if defined(SX_PLATFORM_LINUX) || defined (SX_PLATFORM_WINDOWS) || defined(SX_PLATFORM_MACOS)
    #define SX_OPENGL_SUPPORTED
    #include <glad/glad.h>
#endif

#endif //STRAITX_OPENGL_HPP 