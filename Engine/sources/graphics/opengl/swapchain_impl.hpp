#ifndef STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP
#define STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP

#include "graphics/api/swapchain.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"
#include "graphics/opengl/render_pass_impl.hpp"

namespace StraitX{
namespace GL{

struct SwapchainImpl: public Swapchain{
    GL::RenderPassImpl m_FramebufferPass;
    GL::FramebufferImpl m_DefaultFramebuffer;

    SwapchainImpl(const Window &window, const SwapchainProperties &props);

    virtual const Framebuffer *CurrentFramebuffer()const override;

    virtual const RenderPass *FramebufferPass()const override;

    static Swapchain *NewImpl(const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SWAPCHAIN_IMPL_HPP