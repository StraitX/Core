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
    virtual Result Create(const PhysicalGPU &gpu)override;

    virtual void Destroy()override;
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_LOGICAL_GPU_IMPL_HPP