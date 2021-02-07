#ifndef STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP
#define STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP

#include "graphics/api/swapchain.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{
namespace GL{

class SwapchainImpl: public Swapchain{
private:
    GL::LogicalGPUImpl *m_Owner = nullptr;
public:
    SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    virtual void SwapFramebuffers(GPUContext &context)override;

    static Swapchain *NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP