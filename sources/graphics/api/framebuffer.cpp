#include "graphics/api/framebuffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/framebuffer_impl.hpp"
#endif

Framebuffer *Framebuffer::Create(const FramebufferProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::FramebufferImpl(props);
#endif

    return nullptr;
}