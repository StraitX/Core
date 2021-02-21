#include <new>
#include "platform/memory.hpp"
#include "graphics/opengl/swapchain_impl.hpp"

namespace StraitX{
namespace GL{

static GPUTexture *FakeFramebufferTexturePtr = nullptr;

static AttachmentDescription TempAttachment = {
    GPUTexture::Layout::PresentSrcOptimal,
    GPUTexture::Layout::PresentSrcOptimal,
    GPUTexture::Layout::ColorAttachmentOptimal,
    GPUTexture::Format::Unknown,
    SamplePoints::Samples_1
};

static RenderPassProperties ToFramebufferProperties(const SwapchainProperties &props){
    //XXX not thread-safe
    TempAttachment.Format = props.FramebufferFormat;
    TempAttachment.Samples = props.FramebufferSamples; 
    return {{&TempAttachment, 1}};
}

SwapchainImpl::SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props):
    m_Owner(static_cast<GL::LogicalGPUImpl *>(&gpu)),
    m_FramebufferPass(gpu, ToFramebufferProperties(props)),
    m_DefaultFramebuffer(0, &m_FramebufferPass, { {window.Size().width, window.Size().height}, {&FakeFramebufferTexturePtr, 1} })
{
    // Yes, OpenGL, Here we go
    (void)window;
    (void)props;
}

void SwapchainImpl::SwapFramebuffers(GPUContext *context){
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