#include "core/os/vulkan.hpp"
#include "core/array.hpp"
#include "core/fixed_list.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/api/gpu.hpp"

#include "core/print.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/swapchain_impl.hpp"
#endif

Swapchain *Swapchain::Create(const Window *window){
#if defined (SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::SwapchainImpl(window);
#endif
    return nullptr;
}


FramebufferChain::FramebufferChain(const Window *window, TextureFormat depth_buffer_format){
    m_Swapchain = UniquePtr<Swapchain>(Swapchain::Create(window));

    if(IsDepthFormat(depth_buffer_format)){
        m_DepthBuffer = UniquePtr<Texture2D>(
            Texture2D::Create(m_Swapchain->Size(), depth_buffer_format, TextureUsageBits::DepthStencilOptimal | TextureUsageBits::TransferDst, TextureLayout::DepthStencilAttachmentOptimal)
        );
    }

    FixedList<AttachmentDescription, 2> s_Attachments = {
        {
            TextureLayout::PresentSrcOptimal,
            TextureLayout::ColorAttachmentOptimal,
            TextureLayout::PresentSrcOptimal,
            m_Swapchain->Format(),
            SamplePoints::Samples_1
        }
    };

    if (HasDepth()) {
        s_Attachments.Add({
            TextureLayout::DepthStencilAttachmentOptimal,
            TextureLayout::DepthStencilAttachmentOptimal,
            TextureLayout::DepthStencilAttachmentOptimal,
            m_DepthBuffer->Format(),
            SamplePoints::Samples_1
        });
    }

    m_SwapchainPass = UniquePtr<RenderPass>(RenderPass::Create({s_Attachments}));
    
    CreateFramebuffers();
}

void FramebufferChain::Recreate() {
    m_Framebuffers.Clear();
    m_Swapchain->Recreate();
    if (m_DepthBuffer) {
        m_DepthBuffer = UniquePtr<Texture2D>(
            Texture2D::Create(m_Swapchain->Size(), m_DepthBuffer->Format(), TextureUsageBits::DepthStencilOptimal | TextureUsageBits::TransferDst, TextureLayout::DepthStencilAttachmentOptimal)
        );
    }
    CreateFramebuffers();
}


void FramebufferChain::AcquireNext(const Semaphore *signal_semaphore){
    while(!m_Swapchain->AcquireNext(*signal_semaphore))
        Recreate();
}

void FramebufferChain::PresentCurrent(const Semaphore *wait_semaphore){
    if (!m_Swapchain->PresentCurrent(*wait_semaphore)) {
        wait_semaphore->WaitFor();
        Recreate();
    }
}

void FramebufferChain::CreateFramebuffers() {
    for(Texture2D *image: m_Swapchain->Images()){
        FixedList<Texture2D *, 2> attachments;
        attachments.Add(image);
        if(HasDepth())
            attachments.Add(m_DepthBuffer.Get());

        FramebufferProperties props;
        props.Pass = m_SwapchainPass.Get();
        props.Size = m_Swapchain->Size();
        props.Attachments = attachments;

        m_Framebuffers.Add(Framebuffer::Create(props));
    }
}
