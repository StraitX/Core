#ifndef STRAITX_OPENGL_HPP
#define STRAITX_OPENGL_HPP

#include "core/env/os.hpp"

#if defined(SX_OS_LINUX) || defined (SX_OS_WINDOWS) || defined(SX_OS_MACOS)
    #define SX_OPENGL_SUPPORTED
    #include <glad/glad.h>
#endif

#endif //STRAITX_OPENGL_HPP 