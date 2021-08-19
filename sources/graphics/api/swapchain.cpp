#include "core/os/vulkan.hpp"
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