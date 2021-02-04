#ifndef STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP
#define STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP

#include "platform/glcontext.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{
namespace GL{

class LogicalGPUImpl: public LogicalGPU{
private:
    GLContext m_Context;
public:
    LogicalGPUImpl(const PhysicalGPU &gp);

    ~LogicalGPUImpl();
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP