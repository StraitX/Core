#include <new>
#include "platform/memory.hpp"
#include "graphics/opengl/swapchain_impl.hpp"

namespace StraitX{
namespace GL{


SwapchainImpl::SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props):
    m_Owner(static_cast<GL::LogicalGPUImpl *>(&gpu)),
    m_DefaultFramebuffer(0),
    m_FramebufferPass(gpu, {{&props.ColorAttachmentDescription, 1}})
{
    // Yes, OpenGL, Here we go
    (void)window;
    (void)props;
}

void SwapchainImpl::SwapFramebuffers(GPUContext &context){
    (void)context;
    m_Owner->SwapBuffers();
}

const RenderPass *SwapchainImpl::FramebufferPass(){
    return &m_FramebufferPass;
}

const Framebuffer *SwapchainImpl::CurrentFramebuffer(){
    return &m_DefaultFramebuffer;
}

Swapchain *SwapchainImpl::NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props){
    return new(Memory::Alloc(sizeof(SwapchainImpl))) SwapchainImpl(gpu, window, props);
}

void SwapchainImpl::DeleteImpl(Swapchain *swapchain){
    swapchain->~Swapchain();
    Memory::Free(swapchain);
}


}//namespace GL::
}//namespace StraitX::