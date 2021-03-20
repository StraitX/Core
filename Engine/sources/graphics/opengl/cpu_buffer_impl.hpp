#ifndef STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP
#define STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP

#include "graphics/api/cpu_buffer.hpp"

namespace StraitX{
namespace GL{

struct CPUBufferImpl{
    static void NewImpl(CPUBuffer &buffer, LogicalGPU &owner, u32 size);

    static void DeleteImpl(CPUBuffer &buffer);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_CPU_BUFFER_IMPL_HPP