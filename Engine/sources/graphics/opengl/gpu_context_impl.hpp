#ifndef STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP
#define STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP

#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace GL{

class GPUContextImpl: public GPUContext{
private:
    
public:
    virtual void BeginFrame()override;

    virtual void EndFrame()override;

    virtual void Submit()override;

    virtual void SumbitAsync()override;

    static GPUContext *NewImpl(LogicalGPU &owner);

    static void DeleteImpl(GPUContext *context);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP