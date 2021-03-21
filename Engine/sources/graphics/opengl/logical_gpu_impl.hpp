#ifndef STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP
#define STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP

#include "platform/opengl_context.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{
namespace GL{

struct LogicalGPUImpl: public LogicalGPU{
    OpenGLContext Context;
    s32 MaxTextureUnits;
    s32 MaxUniformBufferBindings;

    static LogicalGPUImpl Instance;

    Result Initialize(const PhysicalGPU &gp)override;

    void Finalize()override;

    void SwapBuffers();
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP