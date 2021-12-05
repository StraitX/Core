#include "core/os/vulkan.hpp"
#include "core/array.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/api/graphics_api.hpp"

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


FramebufferChain::FramebufferChain(const Window *window) {
    m_Swapchain = Swapchain::Create(window);

    Array<AttachmentDescription, 1> s_Attachments = {
        {
            TextureLayout::PresentSrcOptimal,
            TextureLayout::ColorAttachmentOptimal,
            TextureLayout::PresentSrcOptimal,
            m_Swapchain->Format(),
            SamplePoints::Samples_1
        }
    };

    m_SwapchainPass = RenderPass::Create({s_Attachments});

    Recreate();
}

FramebufferChain::~FramebufferChain() {
    for(auto fb: m_Framebuffers)
        delete fb;
    delete m_SwapchainPass;
    delete m_Swapchain;
}

void FramebufferChain::Recreate() {
    m_Framebuffers.Clear();
    for(Texture2D *image: m_Swapchain->Images()){
        FramebufferProperties props;
        props.Pass = m_SwapchainPass;
        props.Size = m_Swapchain->Size();
        props.Attachments = {&image, 1};

        m_Framebuffers.Add(Framebuffer::Create(props));
    }
}
