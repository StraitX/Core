#ifndef STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP
#define STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP

#include "graphics/cpu_buffer.hpp"

namespace GL{

struct CPUBufferImpl{
    static void NewImpl(CPUBuffer &buffer, u32 size);

    static void DeleteImpl(CPUBuffer &buffer);
};

}//namespace GL::

#endif//STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP