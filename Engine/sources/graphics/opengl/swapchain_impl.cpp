#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/swapchain_impl.hpp"

namespace StraitX{
namespace GL{

static const GPUTexture *FakeFramebufferTexturePtr = nullptr;

static AttachmentDescription SwapchainAttachments[] = {
    {
        GPUTexture::Layout::PresentSrcOptimal,
        GPUTexture::Layout::PresentSrcOptimal,
        GPUTexture::Layout::ColorAttachmentOptimal,
        TextureFormat::Unknown,
        SamplePoints::Samples_1,
    },
    {
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        GPUTexture::Layout::DepthStencilAttachmentOptimal,
        TextureFormat::Unknown,
        SamplePoints::Samples_1,
    }
};

static RenderPassProperties ToFramebufferProperties(const SwapchainProperties &props){
    //XXX not thread-safe
    SwapchainAttachments[0].Format = TextureFormat::BGRA8;
    SwapchainAttachments[0].Samples = props.FramebufferSamples; 

    SwapchainAttachments[1].Format = props.DepthFormat;
    return {{SwapchainAttachments, size_t(1 + IsDepthFormat(props.DepthFormat))}};
}

SwapchainImpl::SwapchainImpl(const Window &window, const SwapchainProperties &props):
    Swapchain(props),
    m_FramebufferPass(ToFramebufferProperties(props)),
    m_DefaultFramebuffer(0, &m_FramebufferPass, { {window.Size().width, window.Size().height}, {&FakeFramebufferTexturePtr, 1} })
{
    if(IsDepthFormat(props.DepthFormat)){
        glEnable(GL_DEPTH_TEST); 
        glDepthFunc(GL_LEQUAL);
    }

    // Yes, OpenGL, Here we go
    (void)window;
    (void)props;
}

const Framebuffer *SwapchainImpl::CurrentFramebuffer()const{
    return &m_DefaultFramebuffer;
}

const RenderPass *SwapchainImpl::FramebufferPass()const{
    return &m_FramebufferPass;
}

Swapchain *SwapchainImpl::NewImpl(const Window &window, const SwapchainProperties &props){
    return new(Memory::Alloc(sizeof(SwapchainImpl))) SwapchainImpl(window, props);
}

void SwapchainImpl::DeleteImpl(Swapchain *swapchain){
    swapchain->~Swapchain();
    Memory::Free(swapchain);
}


}//namespace GL::
}//namespace StraitX::