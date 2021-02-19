#ifndef STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP
#define STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP

#include "graphics/api/swapchain.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"
#include "graphics/opengl/render_pass_impl.hpp"

namespace StraitX{
namespace GL{

class SwapchainImpl: public Swapchain{
private:
    GL::LogicalGPUImpl *m_Owner = nullptr;
    GL::RenderPassImpl m_FramebufferPass;
    GL::FramebufferImpl m_DefaultFramebuffer;
public:
    SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    virtual void SwapFramebuffers(GPUContext &context)override;

    virtual const RenderPass *FramebufferPass()override;

    virtual const Framebuffer *CurrentFramebuffer()override;

    static Swapchain *NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP